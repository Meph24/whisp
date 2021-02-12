/*
 * NetGameEventListener.cpp
 *
 *  Created on:	01.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "NetGameEventListener.h"
#include "SyncableManager.h"

NetGameEventListener::NetGameEventListener()
{
}

NetGameEventListener::~NetGameEventListener()
{
	for(auto reg: registeredAt)
	{
		reg->removeNetGameEventListener(this);
	}
}

