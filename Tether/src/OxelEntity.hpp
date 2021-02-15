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

    void drawOxelTree( const OxelTree::Viewer& viewer, const int8_t& mingranularity = -5 )
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
            if( viewer.value().material == Oxel::Material::Rock ) drawCube();
        }
    }

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

	void draw(	const SimClock::time_point& ts, 
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
        float scale = t.oxelWidth(t.root_granularity);
        glScalef( scale, scale, scale );
        rotateDraw( rot.x,  rot.y, rot.z );
        glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);

        OxelTree::Viewer v (t);
        drawOxelTree(v);

        glPopMatrix();
    }


	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();

};

inline void OxelEntity::serialize(sf::Packet& p, bool complete)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline void OxelEntity::deserialize(sf::Packet& p, SyncableManager& sm)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline void OxelEntity::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline void OxelEntity::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline u32 OxelEntity::getClassID()
{
	return CLASS_ID_OxelEntity;
}

#endif // OXELENTITY_HPP
