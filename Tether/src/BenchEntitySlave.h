/*
 * BenchEntitySlave.h
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHENTITYSLAVE_H_
#define SRC_BENCHENTITYSLAVE_H_

#include "BenchAsymSlave.h"
#include "Entity.h"

class BenchEntitySlave: public Entity, public BenchAsymSlave
{
public:
	BenchEntitySlave(spacevec Size, spacevec Pos);
	BenchEntitySlave(AABB boundingBox);
	virtual ~BenchEntitySlave();

	void draw(Timestamp t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);
	void tick(Timestamp t,TickServiceProvider * tsp);
};

#endif /* SRC_BENCHENTITYSLAVE_H_ */
