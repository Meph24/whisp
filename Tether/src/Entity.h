/*
 * Entity.h
 *
 *  Created on:	18.01.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITY_H_
#define SRC_ENTITY_H_
#include "Spacevec.h"
#include "Drawable.h"
class TickServiceProvider;
class Tickable;
#include "Tickable.h"
#include "TickServiceProvider.h"
#include "EntityIdent.h"
#include <vector>
#include "Chunk.h"
#include "AABB.h"

class Entity: public Tickable, Drawable
{
public:
	AABB bb;

	EntityIdent ID;

	spacevec pos;
	spacevec v;

	bool exists=true;//if exists is false, memory will be freed soon [prev description: next tick (enough time for other threads to react)]

	bool isInAir=false;//if true, subject to gravity

	bool selfMovement=false;//if true, moves from own locomotion (true for zombies, cars, humans, false for ballistic projectiles, loot crates, trees)
	void standardMove(float time,ChunkManager * cm);
	virtual void move(float time,ChunkManager * cm);


	//Chunk * mainChunk;//the ticking chunk
	//std::vector<Chunk *> chunks;
	virtual void draw(float tickOffset,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp)=0;

	//time is guaranteed to be between 0 and MAX_TICK_TIME (defined in Tickable.h)
	virtual void tick(float time,TickServiceProvider * tsp)=0;

	void requestDestroy(TickServiceProvider * tsp);

	virtual void onAABBintersect(Entity * other);

	Entity();
	virtual ~Entity();
};

#endif /* SRC_ENTITY_H_ */
