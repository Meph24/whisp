/*
 * BenchAsymMaster.h
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHASYMMASTER_H_
#define SRC_BENCHASYMMASTER_H_

#include "Spacevec.h"
#include "DualPointer.h"

class Entity;
class TickServiceProvider;
class BenchAsymSlave;

class BenchAsymMaster
{
public:
	void interact(Entity * self,DualPointer<BenchAsymSlave> other, float time, TickServiceProvider& tsp);

	BenchAsymMaster();
	virtual ~BenchAsymMaster();
};

#endif /* SRC_BENCHASYMMASTER_H_ */
