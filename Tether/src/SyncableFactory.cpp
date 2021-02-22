/*
 * SyncableFactory.cpp
 *
 *  Created on:	27.01.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "SyncableFactory.h"

#include "Syncable.h"
#include "Zombie_World.h"
#include "Zombie_Enemy.h"
#include "EntityProjectileBulletLike.h"
#include "SyncableManager.h"
#include "ZombieTree.h"

#include "myAssert.h"

SyncableFactory::SyncableFactory()
{
	// TODO Auto-generated constructor stub

}

SyncableFactory::~SyncableFactory()
{
	// TODO Auto-generated destructor stub
}

Syncable* SyncableFactory::createFromPacket(sf::Packet& p,syncID sID, SyncableManager & sm)
{
	Syncable * ret=nullptr;
	u32 classID;
	p>>classID;
	assert(classID!=CLASS_ID_INVALID);//TODO proper error handling
	Entity * asEntity=nullptr;
	Simulation * asSim=nullptr;
	EntityPlayer * asPlayer=nullptr;
	switch(classID)
	{
	case CLASS_ID_Zombie_World:
//		asSim=new Zombie_World(p);//TODO
//		ret=asSim;
		break;
	case CLASS_ID_Zombie_Enemy:
		asEntity=new Zombie_Enemy(p,sm);
		ret=asEntity;
		break;
	case CLASS_ID_EntityProjectileBulletLike:
		asEntity=new EntityProjectileBulletLike(p,sm);
		ret=asEntity;
		break;
	case CLASS_ID_Zombie_Tree:
		asEntity=new Zombie_Tree(p,sm);
		ret=asEntity;
		break;
	case CLASS_ID_EntityPlayer:
		asPlayer=new EntityPlayer(p,sm);
		asEntity=asPlayer;
		ret=asEntity;
		break;
	default:
		bool classIDUndefined=true;
		assert(!classIDUndefined);//TODO proper error handling
	}
	assert(ret);
	ret->sID=sID;
	if(asPlayer)
	{
		//TODO register player to simulation
	}
	else if(asEntity)//delete "else" as soon as plaer is kept as normal entity
	{
		sm.entityNotif.notifyCreation(asEntity);
	}
	if(asSim)
	{
		//TODO set Simulation
	}
	return ret;
}
