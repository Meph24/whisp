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
public:
	std::vector<Entity *> managedEntities;
	WorldDefault(float GridSize);
	virtual ~WorldDefault();

	void requestEntitySpawn(Entity * e);
	void clearEntities();
	void draw(const SimClock::time_point& draw_time,Frustum * viewFrustum,IWorld& iw, Perspective& perspective);
	void tick(const SimClock::time_point& next_tick_begin,TickServiceProvider* tsp);
	void postTick(TickServiceProvider& tsp);
};

#endif /* SRC_WORLDDEFAULT_H_ */
