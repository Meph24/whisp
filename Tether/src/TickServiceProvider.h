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
#include "ICamera3D.h"
class ChunkManager;
class InteractionManager;
#include "Retickable.h"
#include <vector>

//provides services for tickable items
class TickServiceProvider
{
	std::vector<InteractionManager *> interManVec;
	std::vector<Retickable *> retickRequests;
public:
	int tickID=0;
	virtual ICamera3D * getHolderCamera()=0;//can return 0 if currently not held
	virtual ChunkManager * getChunkManager()=0;
	virtual Entity * getTarget(Entity * me)=0;
	std::vector<InteractionManager *> * getInterManVector();//result could later depend on thread

	//initializes the next tick, call once before ticking everyone
	void initNextTick();
	void doReticks();
	void requestRetick(Retickable * e);

	TickServiceProvider();
	virtual ~TickServiceProvider();
};

#include "ChunkManager.h"
#include "Entity.h"
#include "InteractionManager.h"

#endif /* SRC_TICKSERVICEPROVIDER_H_ */
