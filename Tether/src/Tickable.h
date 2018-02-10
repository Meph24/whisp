/*
 * Tickable.h
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_TICKABLE_H_
#define SRC_TICKABLE_H_
class TickServiceProvider;
//#include "TickServiceProvider.h"
class Entity;
class Tickable
{
public:
	virtual void tick(float time,TickServiceProvider * tsp)=0;
	Tickable();
	virtual ~Tickable();
};

#endif /* SRC_TICKABLE_H_ */
