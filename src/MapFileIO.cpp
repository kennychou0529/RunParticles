//
//  MapFileWriter.cpp
//  RunParticles
//
//  Created by Doug Letterman on 8/19/14.
//
//

#include "MapFileIO.h"
#include "MapFileHandler.h"
#include "TrackLayer.h"

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include <QIODevice>
#include <QSaveFile>
#include <QTextStream>
#include <QXmlSimpleReader>
#include <QXmlInputSource>

const QByteArray MapFileIO::_fileHeaderByteArray("RunParticlesMap");

const quint16 MapFileIO::_fileVersion = 1;

QDomElement _encodePoint(QDomDocument &doc, const QString &name,
                         const LonLat &pt)
{
    QDomElement myEl = doc.createElement(name);
    myEl.setAttribute("lattitudeDegrees", pt.lat());
    myEl.setAttribute("longitudeDegrees", pt.lon());
    return myEl;
};

LonLat _getPoint()
{
    LonLat myPt;
    
    return myPt;
}

MapFileIO::MapFileIO(QObject *parent) :
    QObject(parent),
    _dirty(false)
{
    
}

bool
MapFileIO::writeMapFile(bool relativePaths)
{
    // Deprecated: Use exportMap now
    QDomDocument doc("RunParticlesMap");
    QDomElement root = doc.createElement("RunParticlesMap");
    doc.appendChild(root);
    QDomElement trackFileList = doc.createElement("trackFileList");
    root.appendChild(trackFileList);
    QString trackFilePath;
    foreach(trackFilePath, _trackFiles) {
        QDomElement trackFileElem = doc.createElement("trackFile");
        if (relativePaths)
            trackFilePath = relativeizePath(trackFilePath);
        trackFileElem.setAttribute("path", trackFilePath);
        trackFileList.appendChild(trackFileElem);
    }
    QDomElement viewAreaEl = doc.createElement("viewArea");
    viewAreaEl.appendChild(_encodePoint(doc, "upperLeft", _viewArea.upperLeft));
    viewAreaEl.appendChild(_encodePoint(doc, "lowerRight",
                                        _viewArea.lowerRight));
    root.appendChild(viewAreaEl);
    QSaveFile saveFile(_filename);
    saveFile.open(QIODevice::WriteOnly);
    QTextStream ts(&saveFile);
    ts << doc.toString();
    saveFile.commit();
    _dirty = false;
    return true;
}


bool
MapFileIO::exportMap(const LayerPtrList& layers)
{
    QSaveFile saveFile(_filename);
    saveFile.open(QIODevice::WriteOnly);
    QDataStream dataStream(&saveFile);
    dataStream.setVersion( QDataStream::Qt_4_6 );
    dataStream.writeRawData(_fileHeaderByteArray.constData(),
                            _fileHeaderByteArray.size() );
    dataStream << _fileVersion;
    dataStream << _viewArea.upperLeft;
    dataStream << _viewArea.lowerRight;
    QList<Track> tracks;
    LayerPtrList::const_iterator it;
    for (it = layers.begin(); it != layers.end(); it++)
    {
        std::shared_ptr<TrackLayer> myLayer =
        std::dynamic_pointer_cast<TrackLayer>(*it);
        if (myLayer) {
            Track theTrack = *myLayer->getTrack();
            tracks.append(theTrack);
        }
    }
    dataStream << tracks;
    saveFile.commit();
    return true;
}

bool
MapFileIO::loadMapFile(char **whyNot)
{
    QFile *theFile = new QFile(_filename);
    if (!theFile->exists()) {
        if (whyNot) {
            QString tmpWhyNot = QString("'%0' doesn't exist").arg(_filename);
            *whyNot = tmpWhyNot.toLocal8Bit().data();
        }
        return false;
    }
    QXmlSimpleReader reader;
    MapFileHandler handler;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    
    QXmlInputSource source(theFile);
    
    reader.parse(&source, true /*incremental*/);
    while (reader.parseContinue()) { };
    QString trackFilePath;
    foreach(trackFilePath, handler.getTrackFiles()) {
        _trackFiles.append(absoluteizePath(trackFilePath));
    }
    _viewArea = handler.getViewArea();
    return true;
}

bool
MapFileIO::importMapFile(QList<Track*> &tracks, char **whyNot)
{
    QFile *theFile = new QFile(_filename);
    if (!theFile->exists()) {
        if (whyNot) {
            QString tmpWhyNot = QString("'%0' doesn't exist").arg(_filename);
            *whyNot = tmpWhyNot.toLocal8Bit().data();
        }
        return false;
    }
    theFile->open( QIODevice::ReadOnly );
    QDataStream dataStream(theFile);
    dataStream.setVersion( QDataStream::Qt_4_6 );
    const int len  = _fileHeaderByteArray.size();
    QByteArray actualFileHeaderByteArray( len, '\0' );
    dataStream.readRawData( actualFileHeaderByteArray.data(), len );
    
    if ( actualFileHeaderByteArray != _fileHeaderByteArray )
    {
        // prefixes don't match
        theFile->close();
        if ( actualFileHeaderByteArray.startsWith("<!DOCTYPE")) {
            // It looks like the old XML-format list of paths
            return loadMapFile(whyNot);
        }
        qWarning("File prefix mismatch error");
        QString tmpWhyNot = QString( "Map file prefix mismatch error." );
        *whyNot = tmpWhyNot.toLocal8Bit().data();
        return false;
    }
    qDebug("At position: %lld", dataStream.device()->pos());
    quint16 actualFileVersion = 0;
    dataStream >> actualFileVersion;
    qDebug("At position: %lld", dataStream.device()->pos());
    if ( actualFileVersion == 0 || actualFileVersion > _fileVersion )
    {
        // file is from a future version that we don't know how to load
        QString tmpWhyNot = QString("Compatibility error: actualFileVersion = "
                                    "%1 and fileVersion = %2" )
                                    .arg( actualFileVersion )
                                    .arg( _fileVersion );
        qWarning("File version mismatch %d > %d", actualFileVersion,
                 _fileVersion);
        theFile->close();
        return false;
    }
    dataStream >> _viewArea.upperLeft;
    dataStream >> _viewArea.lowerRight;
    QList <Track> trackData;
    dataStream >> trackData;
    Track thisTrack;
    foreach(thisTrack, trackData) {
        Track* trackPtr = new Track();
        trackPtr->name = thisTrack.name;
        trackPtr->sport = thisTrack.sport;
        trackPtr->points = thisTrack.points;
        tracks.append(trackPtr);
    }
    theFile->close();
    return true;
}

void
MapFileIO::addTrackFile(const QString &filename)
{
    _trackFiles.append(filename);
    _dirty = true;
}

void
MapFileIO::setFilename(const QString &filename)
{
    _filename = filename;
}

void
MapFileIO::clear()
{
    _filename.clear();
    _trackFiles.clear();
    _dirty = false;
}

void
MapFileIO::setViewArea(const LonLatBox &bbox)
{
    _viewArea = bbox;
}

QString
MapFileIO::relativeizePath(const QString &path)
{
    QFileInfo mapInfo(_filename);
    QDir mapDir = mapInfo.absoluteDir();
    return mapDir.relativeFilePath(path);
}

QString
MapFileIO::absoluteizePath(const QString &path)
{
    QFileInfo pathInfo(path);
    if (pathInfo.isAbsolute())
        return path;
    QFileInfo mapInfo(_filename);
    QDir mapDir = mapInfo.absoluteDir();
    return mapDir.absoluteFilePath(path);
}

