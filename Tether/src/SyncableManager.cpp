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

SyncableManager::SyncableManager(Simulation & s):
sim(s)
{
	internalSpawn(&s);
}

syncID SyncableManager::getNextID()
{
	syncID ret=nextID;
	nextID++;
	return ret;
}

void SyncableManager::addSyncable(Syncable* s)
{
	syncMap[s->sID]=s;
}

void SyncableManager::fillUpdatePacket(sf::Packet& p, u32 byteBudget)
{
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
		}
	}
}

void SyncableManager::removeSyncable(syncID ID)
{
	syncMap.erase(ID);
}

void SyncableManager::removeSyncable(Syncable* s)
{
	removeSyncable(s->sID);
}

void SyncableManager::fillCompletePacket(sf::Packet& p)
{
	assert(events.empty());//events must be extracted  with fetchEventPackets() before calling fillCompletePacket()
	for(auto tup: syncMap)
	{
		Syncable * s=tup.second;
		createSpawnEvent(s);
	}
	fetchEventPackets(p);
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

void SyncableManager::fetchEventPackets(sf::Packet& p)
{
	for(auto eventPacket: events)
	{
		u32 len=(u32)eventPacket.getDataSize();
		assert(((size_t)len)==eventPacket.getDataSize());
		p<<len;
		p.append(eventPacket.getData(),eventPacket.getDataSize());
	}
	events.clear();
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
			properSpawn(p,sID);
			break;
		case NET_GAME_EVENT_DELETE:
			p>>sID;
			assert(exists(sID));//TODO proper error handling
			properDelete(sID);
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
	sf::Packet& p=createGenericEvent(NET_GAME_EVENT_SPAWN);
	p<<s->sID;
	assert(s->classID!=0);
	p<<s->classID;
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

void SyncableManager::properDelete(syncID sID)
{
}

void SyncableManager::properSpawn(sf::Packet& p,syncID sID)
{

}

void SyncableManager::properSpawn(Syncable* s)
{
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

void SyncableManager::internalDelete(Syncable* s)
{
	assert(exists(s->sID));
	syncMap.erase(s->sID);
}

Syncable* SyncableManager::getNextToUpdate()
{
	auto result=syncMap.lower_bound(updateQueryIndex);
	if(result==syncMap.end())
	{
		updateQueryIndex=0;
		if(syncMap.empty()) return nullptr;
		result=syncMap.lower_bound(updateQueryIndex);
	}
	return result->second;
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
	return sim.world();
}

TickServiceProvider& SyncableManager::getTSP()
{
	return *sim;
}
