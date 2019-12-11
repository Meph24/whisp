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

#include "collisionl2.hpp"

void Collider::interact(Entity * self,DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{
	//l2 collision detection
	collide(other, delta_time, tsp);
}

void Collider::saveState(const State& state)
{
	old_state = state;
}

spacevec Collider::colSavedPos() const
{
	return old_state.pos;
}

spacevec Collider::colSavedV() const
{
	return old_state.v;
}
