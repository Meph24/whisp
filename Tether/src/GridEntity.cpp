#include "GridEntity.hpp"

#include "AABB.h"
#include "FloatSeconds.hpp"
#include "Frustum.h"
#include "IWorld.h"
#include "TickServiceProvider.h"

GridEntity::GridEntity(const Grid<32>& grid) : grid(grid) {}

void GridEntity::tick(	const SimClock::time_point& next_tick_begin,
						TickServiceProvider* tsp
					 )
{
    float tick_seconds = (float) FloatSeconds(next_tick_begin - last_ticked);

    tick_begin_pos = pos;
    tick_begin_v = v;
    tick_begin_rot = rot;
    tick_begin_drot = drot;

    IWorld* iw = &tsp->world();
    
    bb.low = iw->fromMeters(-vec3(2.0f,2.0f,2.0f)) + pos;
    bb.high = iw->fromMeters(vec3(2.0f,2.0f,2.0f)) + pos;

    //simulation testing code
    //TODO remove
    
    if(glm::length(iw->toMeters(pos)) > 500)
        v = -v;

    pos += v*tick_seconds;
    rot += drot* tick_seconds;
    grid.setTransform(		glm::rotate(glm::radians(rot.x), vec3(1,0,0))
                            *	glm::rotate(glm::radians(rot.y), vec3(0,1,0))
                            *	glm::rotate(glm::radians(rot.z), vec3(0,0,1))
    );

/*
    iw->collideAlgo->doChecks(
        (Collider*) this, (Entity*) this,
        tick_seconds, *tsp);
*/
    last_ticked = next_tick_begin;
}

void GridEntity::draw(	const SimClock::time_point& ts, 
                    Frustum* viewFrustum, 
                    IWorld& iw,
                    Perspective& perspective
                    )
{            
    float tickOffset = (float)FloatSeconds(ts-last_ticked);
    //if(!viewFrustum->inside(bb,iw)) return;
    spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
    vec3 interPosMeters = iw.toMeters(interPos);

    glColor3f(1.0f, 0.0f, 0.0f);

    glPushMatrix();

    //apply position
    glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
    
    grid.draw();

    glPopMatrix();
}