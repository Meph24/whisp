/*
 * IWorld.cpp
 *
 *  Created on:	09.09.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "IWorld.h"
#include "Entity.h"
#include "InteractFilterDefaultSym.h"
#include "InteractFilterDefaultAsym.h"
#include "FilterBoxSortAsym.h"
#include "FilterBoxSortSym.h"
#include "FilterHashSym.h"
#include "FilterHashAsym.h"
#include "InteractFilterNoneS.h"
#include "InteractFilterNoneA.h"

IWorld::IWorld()
{
	initAlgos();
}

IWorld::~IWorld()
{
	delete collideAlgo;
	delete projectileAlgo;
	delete pushAlgo;
	delete benchAlgoAsym;
	delete benchAlgoSym;
}

void IWorld::initAlgos()
{
	pushAlgo=new FilterHashSym<Pushable>();//new FilterBoxSortSym<Pushable>();//InteractFilterDefaultSym<Pushable>();//TODO placeholder
	projectileAlgo=new FilterHashAsym<Projectile,Hittable>();//new InteractFilterNoneA<Projectile,Hittable>();//new FilterBoxSortAsym<Projectile,Hittable>(onlySlave);//InteractFilterDefaultAsym<Projectile,Hittable>();//TODO placeholder
	collideAlgo=new FilterBoxSortSym<Collider>();//new InteractFilterNoneS<Collider>();//InteractFilterDefaultSym<Collider>();//TODO placeholder
	benchAlgoSym=new InteractFilterDefaultSym<BenchSym>();
	benchAlgoAsym=new InteractFilterDefaultAsym<BenchAsymMaster,BenchAsymSlave>();
}

float IWorld::toMeters(spacelen l)
{
	float ret=l.floatpart;
	ret+=l.intpart;
	ret*=gridSize;
	return ret;
}
double IWorld::toMetersD(spacelen l)
{
	double ret=l.floatpart;
	ret+=l.intpart;
	ret*=gridSize;
	return ret;
}

vec3 IWorld::toMeters(spacevec v)
{
	vec3 ret=vec3(v.x.floatpart,v.y.floatpart,v.z.floatpart);
	ret+=vec3(v.x.intpart,v.y.intpart,v.z.intpart);
	ret=ret*gridSize;
	return ret;
}

spacelen IWorld::fromMeters(float l) const
{
	l/=gridSize;
	spacelen ret;
	ret.floatpart=l;
	ret.intpart=0;
	ret.correct();
	return ret;
}

spacevec IWorld::fromMeters(const glm::vec3& v) const
{
	spacevec ret;
	ret.x=fromMeters(v.x);
	ret.y=fromMeters(v.y);
	ret.z=fromMeters(v.z);
	return ret;
}

void IWorld::leaveWorld(Entity* e,TickServiceProvider * tsp)
{
	e->onLeaveWorld(tsp);//notify entity that it is leaving the world
	if(e->allowHibernating)
	{
		hibernate(e);
	}
	else
	{
		e->requestDestroy(this);
	}
}

void IWorld::hibernate(Entity* e)
{
	hibernating.push_back(e);
}

void IWorld::wakeHibernating(AABB bb)
{
	while(hibernating.size()>0)
	{
		Entity * e=hibernating.back();
		if(bb.contains(e->pos))
		{
			wakeHibernating(e);
			hibernating.pop_back();
		}
	}
}

void IWorld::wakeHibernating()
{
	while(hibernating.size()>0)
	{
		Entity * e=hibernating.back();
		wakeHibernating(e);
		hibernating.pop_back();
	}
}

void IWorld::wakeHibernating(Entity* e)
{
	requestEntitySpawn(e);
}

void IWorld::requestEntityDelete(Entity* e)
{
	deleteVec.push_back(e);
}

void IWorld::preTick(TickServiceProvider& tsp)
{
	resetAlgos(tsp);
}

#include "myAssert.h"
void IWorld::resetAlgos(TickServiceProvider& tsp)
{
	assert(pushAlgo!=0);
	assert(projectileAlgo!=0);
	assert(collideAlgo!=0);
	assert(benchAlgoAsym!=0);
	assert(benchAlgoSym!=0);
	//TODO make common interface and use a loop instead
	pushAlgo->reset();
	projectileAlgo->reset();
	collideAlgo->reset();
	benchAlgoAsym->reset();
	benchAlgoSym->reset();
	pushAlgo->doPrecalcs(tsp);
	projectileAlgo->doPrecalcs(tsp);
	collideAlgo->doPrecalcs(tsp);
	benchAlgoAsym->doPrecalcs(tsp);
	benchAlgoSym->doPrecalcs(tsp);
}

spacevec IWorld::toUnitLength(spacevec v)
{
	vec3 conv=toMeters(v);
	return fromMeters(conv/glm::length(conv));
}

IWorld::IWorld(float GridSize):
gridSize(GridSize)
{
	initAlgos();
}
void IWorld::finishTick(TickServiceProvider& tsp)
{
	assert(pushAlgo!=0);
	assert(projectileAlgo!=0);
	assert(collideAlgo!=0);
	assert(benchAlgoAsym!=0);
	assert(benchAlgoSym!=0);
	pushAlgo->evaluationPhase(tsp);
	projectileAlgo->evaluationPhase(tsp);
	collideAlgo->evaluationPhase(tsp);
	benchAlgoAsym->evaluationPhase(tsp);
	benchAlgoSym->evaluationPhase(tsp);
}
