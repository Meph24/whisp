/*
 * NetGameEventListener.h
 *
 *  Created on:	01.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_NETGAMEEVENTLISTENER_H_
#define SRC_NETGAMEEVENTLISTENER_H_

#include "ShortNames.h"

#include <vector>

#include <SFML/Network.hpp>

class SyncableManager;


class NetGameEventListener
{
public:
	u32 netGameEventID=0;//override this number with your own eventID //TODO

	std::vector<SyncableManager *> registeredAt;

	NetGameEventListener();
	virtual ~NetGameEventListener();

	virtual void notifyNetGameEvent(sf::Packet p)=0;


	//move and copy protection
	NetGameEventListener(const NetGameEventListener&) = delete;
	NetGameEventListener(NetGameEventListener&&) = delete;
	NetGameEventListener& operator=(const NetGameEventListener&) = delete;
	NetGameEventListener& operator=(NetGameEventListener&&) = delete;

};

#endif /* SRC_NETGAMEEVENTLISTENER_H_ */
