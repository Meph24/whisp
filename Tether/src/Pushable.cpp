/*
 * Pushable.cpp
 *
 *  Created on:	May 3, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "Pushable.h"

void Pushable::interact(Entity * self,DualPointer<Pushable> other, float time, TickServiceProvider* tsp)
{
	ChunkManager * cm=tsp->getChunkManager();
	spacevec dif=self->pos-other.e->pos;
	float difX = cm->toMeters(dif.x);
	float difZ = cm->toMeters(dif.z);
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

		spacevec move=cm->fromMeters(vec3(xP,0,zP)*time);//amplification factor here
		push(move);
		other.pIF->push(-move);
	}
}
Pushable::~Pushable()
{
}
