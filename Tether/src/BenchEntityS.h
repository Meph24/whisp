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

	void draw(Timestamp t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);
	void tick(Timestamp t,TickServiceProvider * tsp);
};

#endif /* SRC_BENCHENTITYS_H_ */
