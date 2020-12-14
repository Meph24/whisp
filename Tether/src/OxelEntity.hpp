#ifndef OXELENTITY_HPP
#define OXELENTITY_HPP

#include "Oxel.hpp"
#include "TickServiceProvider.h"
#include "IWorld.h"
#include "AABB.h"
#include "FloatSeconds.hpp"
#include "Spacevec.h"
#include "Entity.h"

class OxelEntity : public Entity
{
    OxelTree t;

    vec3 rot, drot;
	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;

public:
    OxelEntity( const OxelTree& t ) : t(t) {}

    void tick(	const SimClock::time_point& next_tick_begin,
				TickServiceProvider* tsp
				)
    {
        float tick_seconds = (float) FloatSeconds(next_tick_begin - last_ticked);

        tick_begin_pos = pos;
        tick_begin_v = v;
        tick_begin_rot = rot;
        tick_begin_drot = drot;

        IWorld* iw = tsp->getIWorld();

        float aabblength = t.oxelWidth(t.root_granularity) * 1.5; //approx sqrt of 2
        bb.low = iw->fromMeters(-vec3(aabblength, aabblength, aabblength)) + pos;
        bb.high = iw->fromMeters(vec3(aabblength, aabblength, aabblength)) + pos;

        //simulation testing code
        //TODO remove
        
        if(glm::length(iw->toMeters(pos)) > 500)
            v = -v;

        pos += v*tick_seconds;
        rot += drot* tick_seconds;
        /*
        t.setTransform(		glm::rotate(glm::radians(rot.x), vec3(1,0,0))
                            *	glm::rotate(glm::radians(rot.y), vec3(0,1,0))
                            *	glm::rotate(glm::radians(rot.z), vec3(0,0,1))
        );
        */

    /*
        iw->collideAlgo->doChecks(
            (Collider*) this, (Entity*) this,
            tick_seconds, *tsp);
    */
        last_ticked = next_tick_begin;
    }

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						DrawServiceProvider* dsp
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
       // t.drawHere();

        glPopMatrix();
    }

};


#endif // OXELENTITY_HPP
