/*
 * BenchEntityMaster.h
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHENTITYMASTER_H_
#define SRC_BENCHENTITYMASTER_H_

#include "BenchAsymMaster.h"
#include "Entity.h"

class BenchEntityMaster: public Entity, public BenchAsymMaster
{
public:
	BenchEntityMaster(spacevec Size, spacevec Pos);
	BenchEntityMaster(AABB boundingBox);
	virtual ~BenchEntityMaster();

	void draw(const SimClock::time_point& draw_time,Frustum * viewFrustum,IWorld& iw, Perspective& perspective);
	void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp);
};

#endif /* SRC_BENCHENTITYMASTER_H_ */
