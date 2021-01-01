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
#include "FloatSeconds.hpp"

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

void BenchEntitySlave::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum, IWorld& iw, Perspective& perspective)
{
}
void BenchEntitySlave::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	IWorld * iw=&tsp->world();
	float seconds=(float) FloatSeconds(next_tick_begin - last_ticked);
	last_ticked = next_tick_begin;
	iw->benchAlgoAsym->doChecks((BenchAsymSlave *) this,(Entity *) this,seconds,*tsp);
}
