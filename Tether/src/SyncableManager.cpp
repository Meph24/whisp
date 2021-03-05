/*
 * SyncableManager.cpp
 *
 *  Created on:	28.01.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "SyncableManager.h"

#include "Syncable.h"
#include "myAssert.h"
#include "NetGameEventListener.h"
#include "vectorTools.hpp"
#include "Simulation.hpp"
#include "SyncableFactory.h"

SyncableManager::SyncableManager(Simulation & s)
{
	addSim(&s);
	sender=true;
}

syncID SyncableManager::getNextID()
{
	syncID ret=nextID;
	nextID++;
	return ret;
}

bool SyncableManager::fillUpdatePacket(sf::Packet& p, u32 byteBudget,bool continueLastCall)
{
	bool iDidSomething=false;
	if(!continueLastCall)
	{
		newRound();
	}
	u32 curBytes=0;
	u32 sizeBefore=(u32)p.getDataSize();
	while(curBytes<byteBudget)
	{
		sf::Packet temp;
		Syncable * s=getNextToUpdate();
		if(s==nullptr) break;
		s->serialize(temp,false);
		size_t afterAppend=curBytes+sizeof(syncID)+sizeof(u32)+temp.getDataSize();
		assert(afterAppend==u32(afterAppend));//packet size should NEVER EVER exceed 4GB
		if(afterAppend<=byteBudget)
		{
			p<<s->sID;
			p<<u32(temp.getDataSize());
			p.append(temp.getData(),temp.getDataSize());

			curBytes=afterAppend;
			assert(curBytes==p.getDataSize()-sizeBefore);
			iDidSomething=true;
			justUpdated(s);
		}
	}
	return iDidSomething;
}

bool SyncableManager::fillCompletePacket(sf::Packet& p)
{
	assert(events.empty());//events must be extracted  with fetchEventPackets() before calling fillCompletePacket()
	for(auto tup: syncMap)
	{
		Syncable * s=tup.second;
		createSpawnEvent(s);
	}
	return fetchEventPackets(p);
}

void SyncableManager::applyUpdatePacket(sf::Packet& p)
{
	while(!p.endOfPacket())
	{
		syncID sID;
		u32 subPackLen;
		p>>sID;
		p>>subPackLen;
		if(exists(sID))
		{
			syncMap[sID]->deserialize(p,*this);
		}
		else
		{
			skipBytes(p,subPackLen);//TODO add some kind of debugging message (maybe to F3 screen)
		}
	}
}

bool SyncableManager::exists(syncID sID)
{
	if(syncMap.find(sID) == syncMap.end())
		return false;
	return true;
}

bool SyncableManager::fetchEventPackets(sf::Packet& p)
{
	bool iDidSomething=false;
	for(auto eventPacket: events)
	{
		u32 len=(u32)eventPacket.getDataSize();
		assert(((size_t)len)==eventPacket.getDataSize());
		p<<len;
		p.append(eventPacket.getData(),eventPacket.getDataSize());
		iDidSomething=true;
	}
	events.clear();
	return iDidSomething;
}

void SyncableManager::applyEventPacket(sf::Packet& p)
{
	while(!p.endOfPacket())
	{
		u32 subPackLen;
		p>>subPackLen;
		u32 eventID;
		p>>eventID;
		syncID sID;
		switch(eventID)
		{
		case NET_GAME_EVENT_INVALID:
			assert(eventID!=NET_GAME_EVENT_INVALID);//TODO proper error handling
			break;
		case NET_GAME_EVENT_SPAWN:
			p>>sID;
			assert(!exists(sID));//TODO proper error handling
			{
				Syncable * s=factory->createFromPacket(p,sID,*this);
				if(sender) createSpawnEvent(s);
				internalSpawn(s);
			}
			break;
		case NET_GAME_EVENT_DELETE:
			p>>sID;
			assert(exists(sID));//TODO proper error handling
			{
				if(sender) createDeleteEvent(sID);
				Syncable * s=syncMap[sID];
				internalRemove(s);
				factory->destroyFromPacket(s,*this);
			}
			break;
		default:
			assert(hasEntry(eventID));
			NetGameEventListener * l= listenerMap[eventID];
			l->notifyNetGameEvent(p);
		}
	}
}

void SyncableManager::createSpawnEvent(Syncable* s)
{
	//TODO remove this hack (begin)
	u32 classID=s->getClassID();
	if(classID==CLASS_ID_EntitySound) return;//do not sync sound events for now
	if(classID==CLASS_ID_ModelEntity) return;
	if(classID==CLASS_ID_TransModelEntity) return;
	if(classID==CLASS_ID_GridEntity) return;
	if(classID==CLASS_ID_OxelEntity) return;
	if(classID==CLASS_ID_BenchEntitySlave) return;
	if(classID==CLASS_ID_BenchEntityMaster) return;
	if(classID==CLASS_ID_BenchEntityS) return;
	//TODO remove this hack (end)
	sf::Packet& p=createGenericEvent(NET_GAME_EVENT_SPAWN);
	p<<s->sID;
	assert(classID!=0);
	p<<classID;
	s->serialize(p,true);
}

void SyncableManager::createDeleteEvent(syncID sID)
{
	sf::Packet& p=createGenericEvent(NET_GAME_EVENT_DELETE);
	p<<sID;
}

sf::Packet& SyncableManager::createGenericEvent(u32 eventID)
{
	events.emplace_back();
	sf::Packet& p=events.back();
	p<<eventID;
	return p;
}

void SyncableManager::skipBytes(sf::Packet p,u32 bytes)
{
	u8 dummy;
	for(u32 i=0;i<bytes;i++)
	{
		p>>dummy;
	}
}

void SyncableManager::registerNetGameEventListener(NetGameEventListener* l)
{
	u32 eventID=l->netGameEventID;
	if(hasEntry(eventID))
	{
		removeNetGameEventListener(listenerMap[eventID]);
	}
	listenerMap[eventID]=l;
	l->registeredAt.push_back(this);
}

void SyncableManager::removeNetGameEventListener(NetGameEventListener* l)
{
	u32 eventID=l->netGameEventID;
	if(hasEntry(eventID))
	{
		listenerMap.erase(eventID);
	}
	removeElementUnordered(l->registeredAt,this);
}

bool SyncableManager::hasEntry(u32 netGameEventID)
{
	if(listenerMap.find(netGameEventID) == listenerMap.end())
		return false;
	return true;
}

void SyncableManager::internalSpawn(Syncable* s)
{
	if(!receiver)
	{
		assert(s->sID==0);
		s->sID=getNextID();
	}
	syncMap[s->sID]=s;
}

void SyncableManager::internalRemove(Syncable* s)
{
	assert(exists(s->sID));
	syncMap.erase(s->sID);
}

Syncable* SyncableManager::getNextToUpdate()
{
	if(syncMap.empty()) return nullptr;
	Syncable * next=getNext(updateQueryIndex);
	Syncable * limit=getNext(updateQueryIndexLimit);
	if(next==limit && !oneTimeExceptionGranted)
	{
		return nullptr;
	}
	return next;
}

SyncableManager::~SyncableManager()
{
	while(!listenerMap.empty())
	{
		size_t sizeBefore=listenerMap.size();
		NetGameEventListener * l=listenerMap.begin()->second;
		removeNetGameEventListener(l);
		size_t sizeAfter=listenerMap.size();
		assert(sizeBefore>sizeAfter);
	}
}

IWorld& SyncableManager::getIWorld()
{
	assert(sim);
	return sim->world();
}

Syncable* SyncableManager::getNext(syncID minID) const
{
	if(syncMap.empty()) return nullptr;
	auto result=syncMap.lower_bound(updateQueryIndex);
	if(result==syncMap.end())
	{
		result=syncMap.lower_bound(0);
	}
	return result->second;
}

void SyncableManager::justUpdated(Syncable* s)
{
	updateQueryIndex=s->sID+1;
	oneTimeExceptionGranted=false;
}

void SyncableManager::newRound()
{
	updateQueryIndexLimit=updateQueryIndex;
	oneTimeExceptionGranted=true;
}

TickServiceProvider& SyncableManager::getTSP()
{
	assert(sim);
	return *sim;
}

Simulation* SyncableManager::getSim() const
{
	return sim;
}

SyncableManager::SyncableManager(const WallClock& reference_clock, Cfg& cfg)
: refClock(&reference_clock)
, config(&cfg)
{
	receiver=true;
}


void SyncableManager::notifyCreation(Entity* obj)
{
	assert(!receiver);
	internalSpawn(obj);
	if(sender)
	{
		createSpawnEvent(obj);
	}
}
void SyncableManager::notifyDestruction(Entity* obj)
{
	assert(!receiver);
	internalRemove(obj);
	if(sender)
	{
		createDeleteEvent(obj->sID);
	}
}


void SyncableManager::addSim(Simulation* s)
{
	sim=s;
	if(s==nullptr) return;
	internalSpawn(sim);
	entityNotif.registerCreationDestructionListener(sim->iw.get());
	sim->iw->entityNotif.registerCreationDestructionListener(this);
}

Simulation* SyncableManager::setSim(Simulation* s)
{
	Simulation * old=removeSim();
	addSim(s);
	return old;
}

Simulation* SyncableManager::removeSim()
{
	internalSpawn(sim);
	entityNotif.removeCreationDestructionListener(sim->iw.get());
	sim->iw->entityNotif.removeCreationDestructionListener(this);
	Simulation * ret=sim;
	sim=nullptr;
	return ret;
}
