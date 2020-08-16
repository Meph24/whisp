/*
 * IGameMode.h
 *
 *  Created on:	Dec 11, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_IGAMEMODE_H_
#define SRC_IGAMEMODE_H_

#include "TickServiceProvider.h"
#include "TimestampManager.h"

class EntityPlayer;

class IGameMode: public TickServiceProvider
{
public:
	EntityPlayer * player;
	TimestampManager tm;

	IGameMode();
	virtual ~IGameMode();

	virtual void loop()=0;
	virtual void init()=0;
	virtual void trigger(bool pulled)=0;
};

#endif /* SRC_IGAMEMODE_H_ */
