/*
 * Hittable.cpp
 *
 *  Created on:	04.07.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "Hittable.h"

//#include "InteractionManager.h"
//#include "WarnErrReporter.h"
//#include "ChunkManager.h"

//void Hittable::registerHitCheck(Entity* e,float seconds, TickServiceProvider* tsp)
//{
//	std::vector<InteractionManager *> * vec = tsp->getInterManVector();
//	tsp->getChunkManager()->giveInteractionManagers(e,vec,tsp);
//	int size=vec->size();
//	if(size<=0)
//	{
//		WarnErrReporter::notInitializedErr("no chunks found in interManVec: uninitialized chunk?");
//		std::cout<<e->pos<<" "<<e->exists<<" "<<e->v<<" "<<tsp->getChunkManager()->getMiddleChunk();
//	}
//	for(int i=0;i<size;i++)
//	{
//		tsp->getChunkManager()->activeChunk=(*vec)[i]->chunk;
//		(*vec)[i]->collide.registerInteractionCheck(this,e,seconds,tsp);
//	}
//}

HittableBulletLike* Hittable::asHittableBulletLike()
{
	return 0;//if not overridden, return 0
}

Hittable::~Hittable()
{
}

void Hittable::testHit(std::vector<ProjectileCollision> * collisions,hitType type,DualPointer<Projectile> projectile,TickServiceProvider& tsp)
{
	//default: do nothing
	//override this to enable collision checks
	//if collision occurred then place new ProjectileCollision in collisions vector
}
