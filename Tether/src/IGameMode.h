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
#include "SimClock.hpp"

class EntityPlayer;

class IGameMode: public TickServiceProvider
{
public:
	EntityPlayer * player;

	SimClock clock;

	IGameMode(const WallClock& reference_clock);
	virtual ~IGameMode() = default;

	virtual void loop()=0;
	virtual void init()=0;
	virtual void trigger(bool pulled)=0;
};

#endif /* SRC_IGAMEMODE_H_ */
