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

IWorld::IWorld()
{
	pushAlgo=new InteractFilterDefaultSym<Pushable>();//TODO placeholder
	projectileAlgo=new InteractFilterDefaultAsym<Projectile,Hittable>();//TODO placeholder
}

IWorld::~IWorld()
{
	delete pushAlgo;
	delete projectileAlgo;
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

void IWorld::preTick()
{
	pushAlgo->reset();
	projectileAlgo->reset();
	pushAlgo->doPrecalcs();
	projectileAlgo->doPrecalcs();
}
