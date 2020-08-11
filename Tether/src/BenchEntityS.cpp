/*
 * BenchEntityS.cpp
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchEntityS.h"
#include "IWorld.h"
#include "TickServiceProvider.h"
#include "InteractFilterAlgoSym.h"
BenchEntityS::BenchEntityS(spacevec Size, spacevec Pos)
{
	pos=Pos;
	bb=AABB(Pos,Size*0.5f);
	v.set0();
}

BenchEntityS::~BenchEntityS()
{
}

void BenchEntityS::draw(Timestamp t, Frustum* viewFrustum, IWorld& iw,DrawServiceProvider* dsp)
{
}

void BenchEntityS::tick(Timestamp t, TickServiceProvider* tsp)
{
	assert(tsp);
	IWorld * iw=tsp->getIWorld();
	assert(iw);
	float seconds=t-lastTick;
	lastTick=t;
	assert(iw->benchAlgoSym);
	iw->benchAlgoSym->doChecks((BenchSym *) this,(Entity *) this,seconds,*tsp);
}
