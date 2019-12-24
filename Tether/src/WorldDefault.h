/*
 * WorldDefault.h
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_WORLDDEFAULT_H_
#define SRC_WORLDDEFAULT_H_

#include "IWorld.h"

#include <vector>

class WorldDefault: public IWorld
{
	std::vector<Entity *> managedEntities;
public:
	WorldDefault();
	virtual ~WorldDefault();

	void requestEntitySpawn(Entity * e);
	void clearEntities();
	void tick(Timestamp t,TickServiceProvider * tsp);
	void draw(Timestamp t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);
	void postTick(TickServiceProvider * tsp);
};

#endif /* SRC_WORLDDEFAULT_H_ */
