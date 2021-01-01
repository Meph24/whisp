#ifndef WORLDTERRAINDISPLAYER_HPP
#define WORLDTERRAINDISPLAYER_HPP

#include "ChunkManager.h"
#include "WorldDefault.h"

struct WorldTerrainDisplayer : public WorldDefault
{
    ChunkManager& cm;
    WorldTerrainDisplayer( float chunk_size, ChunkManager& cm )
        : WorldDefault( chunk_size )
        , cm( cm )
    {}

    void draw(  const SimClock::time_point& draw_time, 
                Frustum* viewFrustum, 
                IWorld& iw,
                Perspective& perspective)
    {
        WorldDefault::draw(draw_time, viewFrustum, iw, perspective);
        cm.draw(draw_time, viewFrustum, iw, perspective);
	    cm.render( viewFrustum, perspective );
    }
};

#endif // WORLDTERRAINDISPLAYER_HPP
