/*
 * SyncableFactory.cpp
 *
 *  Created on:	27.01.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "SyncableFactory.h"

#include "Syncable.h"

#include "myAssert.h"

SyncableFactory::SyncableFactory()
{
	// TODO Auto-generated constructor stub

}

SyncableFactory::~SyncableFactory()
{
	// TODO Auto-generated destructor stub
}

Syncable* SyncableFactory::createFromPacket(sf::Packet& p,syncID sID)
{
	Syncable * ret=nullptr;
	u32 classID;
	p>>classID;
	assert(classID!=CLASS_ID_INVALID);//TODO proper error handling
	switch(classID)
	{
	case CLASS_ID_Zombie_World:
		break;
	case CLASS_ID_Zombie_Enemy:
		break;
	case CLASS_ID_EntityProjectileBulletLike:
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		bool classIDUndefined=true;
		assert(!classIDUndefined);//TODO proper error handling
	}
	assert(ret);
	return ret;
}
