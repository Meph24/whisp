/*
 * ServiceProviderItem.h
 *
 *  Created on:	Jan 24, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_TICKSERVICEPROVIDER_H_
#define SRC_TICKSERVICEPROVIDER_H_

class ThreadDataManager;
class EventMapper;
class Entity;
class ChunkManager;
class InteractionManager;
class SoundManager;
class Retickable;
class ICamera3D;
class IWorld;

#include <vector>

//provides services for tickable stuff
class TickServiceProvider
{
	ThreadDataManager * threadDataMan;
	std::vector<Retickable *> retickRequests;
	SoundManager * sm;
public:

	EventMapper * eMap;

	int tickID=0;
	virtual ICamera3D * getHolderCamera()=0;//can return 0 if currently not held
	virtual ChunkManager * getChunkManager()=0;
	virtual IWorld * getIWorld()=0;
	virtual Entity * getTarget(Entity * me)=0;
	std::vector<InteractionManager *> * getInterManVector(unsigned int threadID=0);

	//initializes the next tick, call once before ticking everyone
	void initNextTick();
	void doReticks();
	void requestRetick(Retickable * e);
	SoundManager * getSoundManager();


	TickServiceProvider();
	virtual ~TickServiceProvider();
};

#endif /* SRC_TICKSERVICEPROVIDER_H_ */
