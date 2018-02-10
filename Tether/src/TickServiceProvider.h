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

//provides services for tickable items
class TickServiceProvider
{
public:

	virtual void spawnEntity(Entity * e);//spawns Entity into World
	virtual ICamera3D * getHolderCamera();//can return 0 if currently not held
	virtual ChunkManager * getChunkManager();
	virtual void requestDestroy(Entity * e);

	TickServiceProvider();
	virtual ~TickServiceProvider();
};

#endif /* SRC_TICKSERVICEPROVIDER_H_ */
