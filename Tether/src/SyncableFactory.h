/*
 * SyncableFactory.h
 *
 *  Created on:	27.01.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SYNCABLEFACTORY_H_
#define SRC_SYNCABLEFACTORY_H_

#include "ShortNames.h"
#include "Syncable.h"

#include <SFML/Network.hpp>

class SyncableManager;

class SyncableFactory
{
public:
	SyncableFactory();
	virtual ~SyncableFactory();

	virtual Syncable * createFromPacket(sf::Packet& p,syncID sID, SyncableManager & sm);
	virtual void destroyFromPacket(Syncable * s, SyncableManager & sm);
};

#endif /* SRC_SYNCABLEFACTORY_H_ */
