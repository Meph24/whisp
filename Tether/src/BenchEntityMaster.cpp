/*
 * BenchEntityMaster.cpp
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchEntityMaster.h"
#include "IWorld.h"
#include "TickServiceProvider.h"
#include "InteractFilterAlgoAsym.h"

BenchEntityMaster::BenchEntityMaster(spacevec Size, spacevec Pos)
{
	pos=Pos;
	bb=AABB(Pos,Size*0.5f);
	bbColor.g=0;
	bbColor.b=0;
	v.set0();
}

BenchEntityMaster::BenchEntityMaster(AABB boundingBox)
{
	bb=boundingBox;
	bbColor.g=0;
	bbColor.b=0;
	pos=bb.low+(bb.high-bb.low)*0.5f;
	v.set0();
}

BenchEntityMaster::~BenchEntityMaster()
{
}

void BenchEntityMaster::draw(Timestamp t, Frustum* viewFrustum, IWorld& iw,DrawServiceProvider* dsp)
{
}

void BenchEntityMaster::tick(Timestamp t, TickServiceProvider* tsp)
{
	IWorld * iw=tsp->getIWorld();
	float seconds=t-lastTick;
	lastTick=t;
	iw->benchAlgoAsym->doChecks((BenchAsymMaster *) this,(Entity *) this,seconds,*tsp);
}
