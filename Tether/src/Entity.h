/*
 * Entity.h
 *
 *  Created on:	18.01.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITY_H_
#define SRC_ENTITY_H_

#include "Tickable.h"
#include "Drawable.h"

#include "AABB.h"
#include "Spacevec.h"

class TickServiceProvider;
class Frustum;
class ChunkManager;
class DrawServiceProvider;
class IWorld;

#include <vector>


//#include "EntityIdent.h"

class Entity: public Tickable, Drawable
{
	std::vector<Entity *> follower;
	bool requestedDelete=false;
public:
	bool surviveClearing=false;
	bool allowHibernating=false;//if true, survives being outside the world, being transferrred to a hibernation state

	int refCounter=0;//DO NOT TOUCH


	AABB bb;

//	EntityIdent ID;

	spacevec pos;
	spacevec residentPos;//position that dictates in what chunk this entity belongs, do not touch, managed by chunk/chunkMagager
	spacevec v;

	Timestamp lastTick;//used for timing
	int lastTickID=0;//used for resets

	std::vector<void *> alreadyChecked;


	bool exists=true;//if exists is false, memory will be freed soon

	bool multichunk=false;//must be set when updating aabb for collision to work correctly
	//TODO include into AABB class?

	/*
	bool isInAir=false;//if true, subject to gravity

	bool selfMovement=false;//if true, moves from own locomotion (true for zombies, cars, humans, false for ballistic projectiles, loot crates, trees)
	void standardMove(float time,ChunkManager * cm);
	virtual void move(float time,ChunkManager * cm);
*/

	virtual void draw(Timestamp t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp)=0;

	//time is guaranteed to be between 0 and MAX_TICK_TIME (defined in Tickable.h)
	virtual void tick(Timestamp t,TickServiceProvider * tsp)=0;
	virtual void onLeaveWorld(TickServiceProvider * tsp);//called when outside of loaded chunk area

	void requestDestroy(IWorld * w);//call this to request delete, do NOT delete any other way
	void reset();

	void follow(Entity * e);
	void unfollow(Entity * e);
	virtual void notifyRemoval(Entity * e);//notify followers that the followed entity is being removed



	Entity();
	virtual ~Entity();
};

#endif /* SRC_ENTITY_H_ */
