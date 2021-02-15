/*
 * BenchEntityS.h
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHENTITYS_H_
#define SRC_BENCHENTITYS_H_

#include "BenchSym.h"
#include "Entity.h"

class BenchEntityS: public Entity, public BenchSym
{
public:
	BenchEntityS(spacevec Size, spacevec Pos);
	virtual ~BenchEntityS();

	void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw,Perspective&);
	void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp);

	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();
};

#endif /* SRC_BENCHENTITYS_H_ */
