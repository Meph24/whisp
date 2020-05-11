/*
 * BenchSym.h
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHSYM_H_
#define SRC_BENCHSYM_H_

class Entity;
class TickServiceProvider;
#include "Spacevec.h"
#include "DualPointer.h"

class BenchSym
{
public:
	BenchSym();

	void interact(Entity * self,DualPointer<BenchSym> other, float time, TickServiceProvider& tsp);

	virtual ~BenchSym();
};

#endif /* SRC_BENCHSYM_H_ */
