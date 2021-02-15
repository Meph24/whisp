/*
 * Syncable.h
 *
 *  Created on:	24.01.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SYNCABLE_H_
#define SRC_SYNCABLE_H_

#include "ClassIDs.hpp"

#include <SFML/Network.hpp>

class SyncableManager;


class Syncable
{
public:
	syncID sID=0;//0 means nested Syncable (or someone forgot to request an ID)

//	u32 classID=0;//0 means nested Syncable (or someone forgot to override)
	virtual u32 getClassID()=0;

	Syncable();
	virtual ~Syncable();


	//"complete" includes all information, so the object can be created from scratch
	virtual void serialize(sf::Packet& p,bool complete)=0;


	virtual void deserialize(sf::Packet& p,SyncableManager & sm)=0;

	//for complete deserialization, provide a constructor and call it in the SyncableFactory


	//only valid at this instant; NON-RECURSIVE: only provide those directly owned
	virtual void getOwnedSyncables(std::vector<Syncable *> collectHere)=0;//kill these if this Syncable dies
	//only valid at this instant; NON-RECURSIVE: only provide those directly referenced
	virtual void getReferencedSyncables(std::vector<Syncable *> collectHere)=0;//excludes owned
};

#endif /* SRC_SYNCABLE_H_ */
