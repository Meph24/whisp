/*
 * Entity.h
 *
 *  Created on:	18.01.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITY_H_
#define SRC_ENTITY_H_
class TickServiceProvider;
#include "Tickable.h"
#include "Spacevec.h"
#include "EntityIdent.h"
#include <vector>
#include "Chunk.h"

class Entity: public Tickable, Drawable
{
public:
	EntityIdent ID;

	spacevec pos;
	spacevec v;

	bool exists=true;//if exists is false, memory will be freed next tick (enough time for other threads to react)

	//vec3 aabbOlow;//AABB offset to pos on the low end
	//vec3 aabbOhigh;//AABB offset to pos on the high end//TODO best way?

	//Chunk * mainChunk;//the ticking chunk
	//std::vector<Chunk *> chunks;
	virtual void draw(float tickOffset,spacevec observerPos,ChunkManager * cm,DrawServiceProvider * dsp)=0;

	//time is guaranteed to be between 0 and MAX_TICK_TIME (defined in Tickable.h)
	virtual void tick(float time,TickServiceProvider * tsp)=0;

	void requestDestroy(TickServiceProvider * tsp);

	//virtual void onAABBintersect(Entity * other);
	//bool aabbIntersects(Entity * other);

	//vec3 getRelPosOf(Entity * other);//only position relative, not rotation
	Entity();
	virtual ~Entity();
};





//12.03.2017
/*
#include "vec3.h"
#include "ID.h"
class Entity
{
public:

	ID UID;

	vec3 pos;
	vec3 speed;

	bool markDelete;

	virtual void tick(Entity * past,Entity * future,float sec)=0;

	Entity();
	virtual ~Entity();
};
*/
#endif /* SRC_ENTITY_H_ */
