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
class ITerrain;

#include <vector>
#include <memory>

using std::unique_ptr;

//provides services for tickable stuff
class TickServiceProvider
{
	ThreadDataManager * threadDataMan;
	std::vector<Retickable *> retickRequests;
	SoundManager * sm;
public:
	volatile unsigned long long interactionCounter = 0;//for benchmark purposes
	volatile unsigned long long arbitraryNumber = 0;//for benchmark purposes

	virtual IWorld& world() = 0;
	virtual ITerrain* getITerrain() = 0;
	
	virtual Entity* getTarget( const Entity* ) = 0; //returns the target for an enemy

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
