#ifndef GRIDENTITY_HPP
#define GRIDENTITY_HPP


#include "Spacevec.h"
#include "Entity.h"
#include "Grid.hpp"

struct GridEntity : public Entity
{
    Grid<32> grid;

    vec3 rot, drot;
	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;

    GridEntity( const Grid<32>& grid );

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

inline void GridEntity::serialize(sf::Packet& p, bool complete)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline void GridEntity::deserialize(sf::Packet& p, SyncableManager& sm)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline void GridEntity::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline void GridEntity::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

inline u32 GridEntity::getClassID()
{
	return CLASS_ID_GridEntity;
}

#endif // GRIDENTITY_HPP
