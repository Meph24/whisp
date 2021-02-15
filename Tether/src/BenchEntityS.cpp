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
#include "FloatSeconds.hpp"
BenchEntityS::BenchEntityS(spacevec Size, spacevec Pos)
{
	pos=Pos;
	bb=AABB(Pos,Size*0.5f);
	v.set0();
}

BenchEntityS::~BenchEntityS(){}
void BenchEntityS::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum, IWorld& iw, Perspective& perspective){}

void BenchEntityS::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	assert(tsp);
	IWorld* iw=&tsp->world();
	assert(iw);
	float seconds=(float)FloatSeconds(next_tick_begin-last_ticked);
	last_ticked=next_tick_begin;
	assert(iw->benchAlgoSym);
	iw->benchAlgoSym->doChecks((BenchSym *) this,(Entity *) this,seconds,*tsp);
}

void BenchEntityS::serialize(sf::Packet& p, bool complete)
{
	bool thisIsCalled=true;
	assert(!thisIsCalled);
}

void BenchEntityS::deserialize(sf::Packet& p, SyncableManager& sm)
{
	bool thisIsCalled=true;
	assert(!thisIsCalled);
}

void BenchEntityS::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsCalled=true;
	assert(!thisIsCalled);
}

void BenchEntityS::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsCalled=true;
	assert(!thisIsCalled);
}

u32 BenchEntityS::getClassID()
{
	return CLASS_ID_BenchEntityS;
}
