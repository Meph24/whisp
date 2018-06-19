/*
 * ServiceProviderItem.h
 *
 *  Created on:	Jan 24, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_TICKSERVICEPROVIDER_H_
#define SRC_TICKSERVICEPROVIDER_H_

class Entity;
class TickServiceProvider;
#include "Entity.h"
#include "ICamera3D.h"
#include "ChunkManager.h"
class InteractionManager;
#include "InteractionManager.h"

//provides services for tickable items
class TickServiceProvider
{
	std::vector<InteractionManager *> interManVec;
public:
	int tickID=0;
	virtual void spawnEntity(Entity * e)=0;//spawns Entity into World
	virtual ICamera3D * getHolderCamera()=0;//can return 0 if currently not held
	virtual ChunkManager * getChunkManager()=0;
	virtual void requestDestroy(Entity * e)=0;//do not call yourself, call Entity.requestDestroy(tsp) instead!
	virtual Entity * getTarget(Entity * me)=0;
	std::vector<InteractionManager *> * getInterManVector();//result could later depend on thread

	//initializes the next tick, call once before ticking everyone
	void initNextTick();

	TickServiceProvider();
	virtual ~TickServiceProvider();
};

#endif /* SRC_TICKSERVICEPROVIDER_H_ */
