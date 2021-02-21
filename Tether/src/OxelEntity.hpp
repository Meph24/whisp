#ifndef OXELENTITY_HPP
#define OXELENTITY_HPP

#include "Entity.h"
#include "Oxel.hpp"
#include "Spacevec.h"

class OxelEntity : public Entity
{
    OxelTree t;

    vec3 rot, drot;
	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;

    void drawOxelTree( const OxelTree::Viewer& viewer, const int8_t& mingranularity = -5 );
    
public:
    OxelEntity( const OxelTree& t );

    void tick(	const SimClock::time_point& next_tick_begin,
				TickServiceProvider* tsp
				);
    

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						Perspective& perspective
					 );

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
