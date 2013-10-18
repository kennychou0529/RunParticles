#ifndef __TRACKLAYER_H__
#define __TRACKLAYER_H__

#include "Layer.h"
#include "Track.h"
#include "Types.h"

class TrackLayer : public Layer
{
public:
    TrackLayer(const Track *track);
    
    virtual ~TrackLayer();
    
    virtual void draw(const ViewCtx*, const TimeCtx*);
protected:
    
    const Track *_track;
};

#endif;
