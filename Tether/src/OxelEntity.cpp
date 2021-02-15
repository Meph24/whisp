#include "OxelEntity.hpp"

#include "drawUtils.hpp"
#include "FloatSeconds.hpp"
#include "Frustum.h"
#include "IWorld.h"
#include "TickServiceProvider.h"

void OxelEntity::drawOxelTree( const OxelTree::Viewer& viewer, const int8_t& mingranularity )
{
    if( viewer.granularity < mingranularity ) return;
    if( !viewer.node().isLeaf() )
    {
        glPushMatrix();
        glScalef( 0.5, 0.5, 0.5 );
        for (size_t i = 0; i < 8; i++)
        {
            glPushMatrix();
            glTranslatef( 
                (i & 0x1) ? 0.5 : -0.5, 
                (i & 0x2) ? 0.5 : -0.5,
                (i & 0x4) ? 0.5 : -0.5 
            );
            drawOxelTree( viewer.child(i), mingranularity );
            glPopMatrix();
        }
        glPopMatrix();
    }
    else
    {
        viewer.value().draw();
    }
}

OxelEntity::OxelEntity( const OxelTree& t ) : t(t) {}

void OxelEntity::tick( const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp )
{
    float tick_seconds = (float) FloatSeconds(next_tick_begin - last_ticked);

    tick_begin_pos = pos;
    tick_begin_v = v;
    tick_begin_rot = rot;
    tick_begin_drot = drot;

    IWorld* iw = &tsp->world();

    float aabblength = t.oxelWidth(t.root_granularity) * 1.5; //approx sqrt of 2
    bb.low = iw->fromMeters(-vec3(aabblength, aabblength, aabblength)) + pos;
    bb.high = iw->fromMeters(vec3(aabblength, aabblength, aabblength)) + pos;

    //simulation testing code
    //TODO remove
    
    if(glm::length(iw->toMeters(pos)) > 500)
        v = -v;

    pos += v*tick_seconds;
    rot += drot* tick_seconds;

    last_ticked = next_tick_begin;
}

void OxelEntity::draw(	const SimClock::time_point& ts, 
                    Frustum* viewFrustum, 
                    IWorld& iw,
                    Perspective& perspective )
{            
        float tickOffset = (float)FloatSeconds(ts-last_ticked);
        //if(!viewFrustum->inside(bb,iw)) return;
        spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
        vec3 interPosMeters = iw.toMeters(interPos);

        glColor3f(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        //apply position
        float scale = t.oxelWidth(t.root_granularity);
        glScalef( scale, scale, scale );
        rotateDraw( rot.x,  rot.y, rot.z );
        glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);

        OxelTree::Viewer v (t);
        drawOxelTree(v);

        glPopMatrix();
    }