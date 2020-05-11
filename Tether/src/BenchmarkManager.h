/*
 * BenchmarkManager.h
 *
 *  Created on:	04.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHMARKMANAGER_H_
#define SRC_BENCHMARKMANAGER_H_

#include "Tickable.h"

class IWorld;

class BenchmarkManager: public Tickable
{
	IWorld * world;
public:
	void tick(Timestamp t,TickServiceProvider * tsp);//call between ticks, when it is safe to modify required values

	BenchmarkManager(IWorld * World);
	virtual ~BenchmarkManager();
};

#endif /* SRC_BENCHMARKMANAGER_H_ */
