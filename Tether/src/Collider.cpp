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

void Collider::interact(Entity * self,DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{
	//l2 collision detection
	collide(other, delta_time, tsp);
}
