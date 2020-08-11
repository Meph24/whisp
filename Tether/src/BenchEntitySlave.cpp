/*
 * BenchEntitySlave.cpp
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchEntitySlave.h"
#include "IWorld.h"
#include "TickServiceProvider.h"
#include "InteractFilterAlgoAsym.h"

BenchEntitySlave::BenchEntitySlave(spacevec Size, spacevec Pos)
{
	pos=Pos;
	bb=AABB(Pos,Size*0.5f);
	bbColor.r=0;
	bbColor.b=0;
	v.set0();
}

BenchEntitySlave::BenchEntitySlave(AABB boundingBox)
{
	bb=boundingBox;
	pos=bb.low+(bb.high-bb.low)*0.5f;
	bbColor.r=0;
	bbColor.b=0;
	v.set0();
}

BenchEntitySlave::~BenchEntitySlave()
{
}

void BenchEntitySlave::draw(Timestamp t, Frustum* viewFrustum, IWorld& iw,DrawServiceProvider* dsp)
{
}
void BenchEntitySlave::tick(Timestamp t, TickServiceProvider* tsp)
{
	IWorld * iw=tsp->getIWorld();
	float seconds=t-lastTick;
	lastTick=t;
	iw->benchAlgoAsym->doChecks((BenchAsymSlave *) this,(Entity *) this,seconds,*tsp);
}
