/*
 * CollisionGroup.h
 *
 *  Created on:	06.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_COLLISIONGROUP_H_
#define SRC_COLLISIONGROUP_H_
#include <vector>
class Entity;
#include "Entity.h"
#include "TickServiceProvider.h"
class CollisionGroup
{
public:
	std::vector<Entity *> registered;
	void registerCollisionCheck(Entity * e, float time,TickServiceProvider* tsp);
	void check(Entity * e,Entity * r,float time,TickServiceProvider* tsp);
	void reset();

	CollisionGroup();
	~CollisionGroup();
};

#endif /* SRC_COLLISIONGROUP_H_ */
