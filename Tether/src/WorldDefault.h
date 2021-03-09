/*
 * WorldDefault.h
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_WORLDDEFAULT_H_
#define SRC_WORLDDEFAULT_H_

#include <vector>

#include "IWorld.h"

class WorldDefault: public IWorld
{
	std::vector<Entity *> managedEntities;
public:
	WorldDefault(float GridSize);
	virtual ~WorldDefault();

	void clearEntities();
	void draw(const SimClock::time_point& draw_time,Frustum * viewFrustum,IWorld& iw, Perspective& perspective);
	void tick(const SimClock::time_point& next_tick_begin,TickServiceProvider* tsp);
	void postTick(TickServiceProvider& tsp);


	void notifyCreation(Entity * obj);
	void notifyDestruction(Entity * obj);

	void getOwnedSyncables(std::vector<Syncable *> collectHere);
};

#endif /* SRC_WORLDDEFAULT_H_ */
