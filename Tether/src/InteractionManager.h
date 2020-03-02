/*
 * InteractionManager.h
 *
 *  Created on:	19.05.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_INTERACTIONMANAGER_H_
#define SRC_INTERACTIONMANAGER_H_

#include "InteractionGroup1.h"
#include "InteractionGroup2.h"
#include "WarnErrReporter.h"

class Pushable;
class Projectile;
class Hittable;

class InteractionManager
{
public:
	InteractionGroup1<Pushable> push;
	InteractionGroup2<Projectile,Hittable> collide;

	spacevec chunk;//for debug only

	void resetAll();


	InteractionManager(spacevec myChunk,void * filterAlgo,bool multichunkInitValue=false);
	~InteractionManager();
};



#endif /* SRC_INTERACTIONMANAGER_H_ */
