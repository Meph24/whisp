/*
 * InteractionManager.h
 *
 *  Created on:	19.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTIONMANAGER_H_
#define SRC_INTERACTIONMANAGER_H_
#include "Pushable.h"
class Projectile;
#include "Projectile.h"
#include "Hittable.h"
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

#endif /* SRC_INTERACTIONMANAGER_H_ */
