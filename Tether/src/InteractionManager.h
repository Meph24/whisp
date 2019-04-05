/*
 * InteractionManager.h
 *
 *  Created on:	19.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTIONMANAGER_H_
#define SRC_INTERACTIONMANAGER_H_
class Pushable;
class Projectile;
class Hittable;
#include "InteractionGroup1.h"
#include "InteractionGroup2.h"
class InteractionManager
{
public:
	InteractionGroup1<Pushable> push;
	InteractionGroup2<Projectile,Hittable> collide;

	spacevec chunk;//for debug only

	void resetAll();

	InteractionManager(spacevec myChunk);
	~InteractionManager();
};

#include "Projectile.h"
#include "Pushable.h"
#include "Hittable.h"
#endif /* SRC_INTERACTIONMANAGER_H_ */
