/*
 * Tickable.h
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_TICKABLE_H_
#define SRC_TICKABLE_H_

#include "SimClock.hpp"

#define MAX_TICK_TIME 0.02f

class TickServiceProvider;
class Entity;

class Tickable
{
public:
	//difference to last timestamp is guaranteed to be between 0 and MAX_TICK_TIME
	virtual void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp)=0;

	Tickable();
	virtual ~Tickable();
};

#endif /* SRC_TICKABLE_H_ */
