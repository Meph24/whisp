/*
 * Projectile.cpp
 *
 *  Created on:	04.07.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "Projectile.h"

#include "WarnErrReporter.h"
#include "EntityProjectileBulletLike.h"
#include "TickServiceProvider.h"
#include "InteractionManager.h"
#include "Hittable.h"

#include <algorithm>

void Projectile::interact(Entity* self, DualPointer<Hittable> other, float time,TickServiceProvider& tsp)
{
	if(typeH==0)
	{
		WarnErrReporter::notInitializedErr("Projectile has no type flag set, ignoring this collision");
		return;
	}
	if(other.pIF->acceptedConversions==0)
	{
		WarnErrReporter::notInitializedErr("Hittable has no type flag set, ignoring this collision");
		return;
	}

	hitType match=typeH&other.pIF->acceptedConversions;//per definition only 0 or 1 bits can be set
	if(match==0) return;
	int before=collisions.size();
	other.pIF->testHit(&collisions,match,DualPointer<Projectile>(self,this),tsp);
	int after=collisions.size();
	if(after) if(!before) tsp.requestRetick((Retickable *)this);
}


void Projectile::retick(TickServiceProvider* tsp)
{
	int size=collisions.size();
//	std::cout<<"before:"<<std::endl;
	//for(int i=0;i<size;i++)
	//{
	//	std::cout<<collisions[i].location<<"  "<<collisions[i].originChunk.x.intpart<<";"<<collisions[i].originChunk.z.intpart<<"    "<<collisions[i].hitVictim.e<<std::endl;
	//}
	std::stable_sort(collisions.begin(), collisions.end());
	//std::cout<<"after:"<<std::endl;
	//for(int i=0;i<size;i++)
	//{
	//	std::cout<<collisions[i].location<<"  "<<collisions[i].originChunk.x.intpart<<";"<<collisions[i].originChunk.z.intpart<<"    "<<collisions[i].hitVictim.e<<std::endl;
	//}
	for(int i=0;i<size;i++)
	{
		bool cont;
		switch(typeH)
		{
		case FLAG_HIT_TYPE_BULLET_LIKE:
			{
				HittableBulletLike * hittable=collisions[i].hitVictim.pIF->asHittableBulletLike();
				EntityProjectileBulletLike * projectile=asProjectileBulletLike();

				if(projectile==0)
				{
					WarnErrReporter::wrongTypeErr("projectile conversion failed");
					break;
				}
				cont=projectile->collide(hittable,collisions[i],tsp);

				break;
			}
//		case FLAG_HIT_TYPE_INTERACT:
//			break;
		default:
			WarnErrReporter::unknownTypeErr("Projectile has unknown type flag, ignoring this collision");
			return;
		}

		if(!cont) break;
	}
	collisions.clear();
}

//#include "WarnErrReporter.h"
//void Projectile::registerHitCheck(Entity* e, float seconds,TickServiceProvider* tsp)
//{
//	std::vector<InteractionManager *> * vec = tsp->getInterManVector();
//	tsp->getChunkManager()->giveInteractionManagers(e,vec,tsp);
//	int size=vec->size();
//	if(size<=0) WarnErrReporter::notInitializedErr("no chunks found in interManVec: uninitialized chunk?");
//	for(int i=0;i<size;i++)
//	{
//		(*vec)[i]->collide.registerInteractionCheck(this,e,seconds,tsp);
//	}
//}

Projectile::~Projectile()
{
}

EntityProjectileBulletLike* Projectile::asProjectileBulletLike()
{
	return 0;
}
