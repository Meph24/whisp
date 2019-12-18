/*
 * Pushable.cpp
 *
 *  Created on:	May 3, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "Pushable.h"
#include "TickServiceProvider.h"
#include "Entity.h"
#include "InteractionManager.h"
#include "ChunkManager.h"

#include "glmutils.hpp"
#include <glm/glm.hpp>

using glm::vec3;

void Pushable::interact(Entity * self,DualPointer<Pushable> other, float time, TickServiceProvider& tsp)
{
	IWorld * iw=tsp.getIWorld();
	spacevec dif=self->pos-other.e->pos;
	float difX = iw->toMeters(dif.x);
	float difZ = iw->toMeters(dif.z);
	float dist;

	float totRad =  pushRadius + other.pIF->pushRadius;
	float totForce= pushForce + other.pIF->pushForce;

	if ((dist = (difX*difX + difZ*difZ)) < totRad*totRad)
	{
		float minR = pushRadius < other.pIF->pushRadius ?  pushRadius : other.pIF->pushRadius;
		dist = sqrt(dist);

		float f;
		if (dist < (totRad - minR*0.975f)) f = 40 * totForce;
		else f = minR / (dist - (totRad - minR))*totForce;
		if (dist < (totRad / 1024.0f))
		{
			difX = (totRad / 1024.0f);
			dist = (totRad / 1024.0f);
		}
		f/=dist;
		float xP = f*difX;
		float zP = f*difZ;

		spacevec move=iw->fromMeters(vec3(xP,0,zP)*time);//amplification factor here
		push(move,tsp);
		other.pIF->push(-move,tsp);
	}
}

//void Pushable::registerPushCheck(Entity* e,float seconds, TickServiceProvider* tsp)
//{
//	std::vector<InteractionManager *> * vec = tsp->getInterManVector();
//	tsp->getChunkManager()->giveInteractionManagers(e,vec,tsp);
//	int size=vec->size();
//	for(int i=0;i<size;i++)
//	{
//		(*vec)[i]->push.registerInteractionCheck(this,e,seconds,tsp);
//	}
//}

Pushable::~Pushable()
{
}
