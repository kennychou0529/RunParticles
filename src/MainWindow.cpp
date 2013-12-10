#include "MainWindow.h"

#include <QApplication>
#include <QEventLoop>
#include <QFileDialog>
#include <QGridLayout>
#include <QMenu>
#include <QMessageBox>
#include <QWidget>

#include "TcxHandler.h"
#include "TrackLayer.h"
#include "TrackFileReader.h"
#include "Util.h"

MainWindow::MainWindow(GLWidget *glWidget,
                       QWidget * parent,
                       Qt::WindowFlags flags) :
    QMainWindow::QMainWindow(parent, flags),
    _menuBar(new QMenuBar(0)),
    _glWidget(glWidget),
    _layerListWidget(new LayerListWidget())
{
    QWidget *ctr = new QWidget(this);
    setCentralWidget(ctr);
    _rewindButton = new QPushButton("<<", ctr);
    _backButton = new QPushButton("<", ctr);
    _pauseButton = new QPushButton("x",ctr);
    _forwardButton = new QPushButton(">", ctr);
    _playSpeedCombo = new QComboBox(ctr);
    _playSpeedCombo->addItem("1x");
    _playSpeedCombo->addItem("2x");
    _playSpeedCombo->addItem("4x");
    _playSpeedCombo->addItem("8x");
    _playSpeedCombo->addItem("16x");
    _playSpeedCombo->addItem("32x");
    _playSpeedCombo->addItem("64x");
    _slider = new QSlider(Qt::Horizontal, ctr);
    _currentTimeLineEdit = new QLineEdit(ctr);
    _currentTimeLineEdit->setReadOnly(true);
    _currentTimeLineEdit->setText("0");
    _currentTimeLineEdit->setAlignment(Qt::AlignRight);
    _layout(ctr);
    QMenu *_fileMenu = _menuBar->addMenu("File");
    _openLayerAction = new QAction("&Open Layer. . .", this);
    _forwardAction = new QAction("Play", this);
    _backAction = new QAction("Reverse", this);
    _rewindAction = new QAction("Rewind", this);
    _pauseAction = new QAction("Pause", this);
    _fileMenu->addAction(_openLayerAction);
    connect(_openLayerAction, SIGNAL(triggered(bool)),
            this, SLOT(slotLoadFile()));
    connect(_forwardButton, SIGNAL(clicked(bool)),
            _forwardAction, SLOT(trigger()));
    connect(_backButton, SIGNAL(clicked(bool)),
            _backAction, SLOT(trigger()));
    connect(_pauseButton, SIGNAL(clicked(bool)),
            _pauseAction, SLOT(trigger()));
    connect(_rewindButton, SIGNAL(clicked(bool)),
            _rewindAction, SLOT(trigger()));
    connect(_playSpeedCombo, SIGNAL(activated(const QString&)),
            this, SLOT(slotPlaybackRateChanged(const QString&)));
    
    connect(_forwardAction, SIGNAL(triggered()),
            _glWidget, SLOT(slotPlay()));
    connect(_backAction, SIGNAL(triggered()),
            _glWidget, SLOT(slotReverse()));
    connect(_pauseAction, SIGNAL(triggered()),
            _glWidget, SLOT(slotPause()));
    connect(_rewindAction, SIGNAL(triggered()),
            _glWidget, SLOT(slotRewind()));
    
    connect(_glWidget, SIGNAL(signalTimeChanged(double)),
            this, SLOT(slotTimeChanged(double)));
    connect(_glWidget->getMap(), SIGNAL(signalLayerAdded()),
            this, SLOT(slotLayerAdded()));
    
    // Application-wide shortcuts
    _playPauseShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    _playPauseShortcut->setContext(Qt::ApplicationShortcut);
    connect(_playPauseShortcut, SIGNAL(activated()),
            _glWidget, SLOT(slotTogglePlayPause()));
    _openLayerShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    _openLayerShortcut->setContext(Qt::ApplicationShortcut);
    connect(_openLayerShortcut, SIGNAL(activated()),
            _openLayerAction, SLOT(trigger()));
    
    connect(_layerListWidget, SIGNAL(signalLayersSelected(QList<unsigned int>)),
            this, SLOT(slotLayerSelectionChanged(const QList<unsigned int>)));
    
    slotTimeChanged(0);
    _layerListWidget->show();
    
    // DEBUG
    QString pathOne("/Users/dal/Dropbox/tmp/10-8-13 6-29-18 PM.tcx");
    loadTrackFile(pathOne);
    QString pathTwo("/Users/dal/Documents/gps/exports/all2012.tcx");
    loadTrackFile(pathTwo);
    QString pathThree("/Users/dal/Documents/gps/exports/2013_to1028.tcx");
    loadTrackFile(pathThree);
    
}

MainWindow::~MainWindow()
{
    // empty
}

void
MainWindow::_layout(QWidget *ctr)
{
    QGridLayout *_layout = new QGridLayout(ctr);
    _layout->addWidget(_rewindButton, 0, 0);
    _layout->addWidget(_backButton, 0, 1);
    _layout->addWidget(_pauseButton, 0, 2);
    _layout->addWidget(_forwardButton, 0, 3);
    _layout->addWidget(_playSpeedCombo, 0, 4);
    _layout->addWidget(_slider, 1, 0, 1, 4);
    _layout->addWidget(_currentTimeLineEdit, 1, 4);
    for (int i=0; i < _layout->columnCount(); i++)
        _layout->setColumnStretch(i, 1);
}

void
MainWindow::loadTrackFile(const QString &path)
{
    QList<Track*> tracks;
    char* whyNot = (char*)malloc(256);
    TrackFileReader reader;
    bool success = reader.read(path, &tracks, &whyNot);
    if (!success) {
        QMessageBox::critical(this, "Could not load file", whyNot);
        return;
    }
    Track *thisTrack;
    foreach(thisTrack, tracks) {
        TrackLayer *thisLayer = new TrackLayer(thisTrack);
        _glWidget->getMap()->addLayer(thisLayer);
        _layerListWidget->addLayer(thisLayer);
    }
}

void
MainWindow::slotLoadFile()
{
    QString path = QFileDialog::getOpenFileName(this, 
                                                "Select track file",
                                                QString() /*dir*/, 
                                                "Tracklogs (*.gpx *.tcx)");
    if (!path.isEmpty()) {
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        loadTrackFile(path);
    }
}

void
MainWindow::slotPlaybackRateChanged(const QString &newRate)
{
    QString rate(newRate);
    if (rate.endsWith("x"))
        rate.chop(1);
    bool ok = false;
    double theRate = rate.toDouble(&ok);
    if (ok)
        _glWidget->setPlaybackRate(theRate);
}

void
MainWindow::slotTimeChanged(double mapSeconds)
{
    QString time = Util::secondsToString(mapSeconds);
    _currentTimeLineEdit->setText(time);
    _slider->setSliderPosition(int(mapSeconds));
}

void
MainWindow::slotLayerAdded()
{
    int dur = _glWidget->getMap()->getDuration();
    _slider->setMaximum(dur);
}

void
MainWindow::slotLayerSelectionChanged(const QList<unsigned int> layerIds)
{
    
}
