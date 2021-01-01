/*
 * BenchEntityS.h
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHENTITYS_H_
#define SRC_BENCHENTITYS_H_

#include "BenchSym.h"
#include "Entity.h"

class BenchEntityS: public Entity, public BenchSym
{
public:
	BenchEntityS(spacevec Size, spacevec Pos);
	virtual ~BenchEntityS();

	void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw,Perspective&);
	void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp);
};

#endif /* SRC_BENCHENTITYS_H_ */
