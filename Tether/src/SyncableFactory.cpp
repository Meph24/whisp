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
	std::cout<<"factory creates object with class="<<className(classID)<<" and sID="<<sID<<std::endl;
	assert(classID!=CLASS_ID_INVALID);//TODO proper error handling
	Entity * asEntity=nullptr;
	Simulation * asSim=nullptr;
	EntityPlayer * asPlayer=nullptr;
	switch(classID)
	{
	case CLASS_ID_Zombie_World:
		assert(sm.refClock);
		assert(sm.config);
		asSim=new Zombie_World(*sm.refClock, *sm.config);
		ret=asSim;
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
	//if(asPlayer) {} else
	if(asEntity)
	{
		sm.entityNotif.notifyCreation(asEntity);
	}
	if(asSim)
	{
		Simulation * prev=sm.setSim(asSim);
		assert(prev==nullptr);
	}
	return ret;
}

void SyncableFactory::destroyFromPacket(Syncable * s, SyncableManager& sm)
{
	Entity * asEntity=nullptr;
	Simulation * asSim=nullptr;
	//EntityPlayer * asPlayer=nullptr;
	u32 classID=s->getClassID();
	switch(classID)
	{
	case CLASS_ID_Zombie_World:
	case CLASS_ID_Simulation_World:
		asSim=(Simulation *) s;
		break;
	case CLASS_ID_EntityPlayer:
		//asPlayer=(EntityPlayer *)s;
	case CLASS_ID_Zombie_Enemy:
	case CLASS_ID_EntityProjectileBulletLike:
	case CLASS_ID_Zombie_Tree:
	case CLASS_ID_EntitySound:
	case CLASS_ID_ModelEntity:
	case CLASS_ID_TransModelEntity:
	case CLASS_ID_GridEntity:
	case CLASS_ID_OxelEntity:
	case CLASS_ID_BenchEntitySlave:
	case CLASS_ID_BenchEntityMaster:
	case CLASS_ID_BenchEntityS:
		asEntity=(Entity *)s;
		break;
	default:
		bool classIDUndefined=true;
		assert(!classIDUndefined);//TODO proper error handling
	}
	//if(asPlayer) {} else
	if(asEntity)
	{
		sm.entityNotif.notifyDestruction(asEntity);
	}
	if(asSim)
	{
		Simulation * prev=sm.setSim(nullptr);
		assert(prev!=nullptr);
		assert(prev==asSim);
	}
	std::vector<Syncable * > testVec;
	s->getOwnedSyncables(testVec);
	assert(testVec.empty());
	delete s;
}
