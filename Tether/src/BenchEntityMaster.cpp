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
#include "FloatSeconds.hpp"

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

void BenchEntityMaster::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum, IWorld& iw,DrawServiceProvider* dsp)
{
}

void BenchEntityMaster::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	IWorld * iw=tsp->getIWorld();
	float seconds= (float) FloatSeconds(next_tick_begin - last_ticked);
	last_ticked = next_tick_begin;
	iw->benchAlgoAsym->doChecks((BenchAsymMaster *) this,(Entity *) this,seconds,*tsp);
}
