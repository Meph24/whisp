/*
 * Collider.cpp
 *
 *  Created on:	May 3, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "Collider.hpp"
#include "TickServiceProvider.h"
#include "Entity.h"
#include "InteractionManager.h"
#include "ChunkManager.h"
#include "FloatSeconds.hpp"

void Collider::interact(Entity * self,DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{
	//l2 collision detection
	microseconds tick_duration = duration_cast<microseconds>(FloatSeconds (delta_time));
	collide(other, tick_duration, tsp);
}
