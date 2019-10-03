/*
 * IWorld.h
 *
 *  Created on:	09.09.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_IWORLD_H_
#define SRC_IWORLD_H_

#include "Tickable.h"
#include "Drawable.h"

#include "AABB.h"
#include "InteractionManager.h"

#include <vector>

class IWorld: public Tickable, public Drawable
{
	std::vector<Entity *> hibernating;

protected:

	float gridSize;//sacelen.intpart scale in meters
	std::vector<Entity *> deleteVec;//the entities that should be removed from world


public:
	IWorld();
	virtual ~IWorld();

	//only allowed for RELATIVE positions/velocities:
	float toMeters(spacelen l);
	double toMetersD(spacelen l);
	vec3 toMeters(spacevec v);
	spacelen fromMeters(float l) const;
	spacevec fromMeters(const vec3& v) const;

	virtual void leaveWorld(Entity * e,TickServiceProvider * tsp);//is called if Entity is outside loaded area
	virtual void hibernate(Entity * e);//Entity outside loaded area, but not deleted, does not get ticked
	virtual void wakeHibernating(AABB bb);//awakes hibernating Entities inside bb and tries to insert them into active area
	virtual void wakeHibernating();//awakes all hibernating Entities and tries to insert them into active area
	virtual void wakeHibernating(Entity * e);//awakes this hibernating Entity and tries to insert it into active area, returns 0 if successful, e otherwise

	virtual void requestEntitySpawn(Entity * e)=0;//spawn entity in world, call only once per entity!!! Can fail if not within loaded chunks

	virtual void requestEntityDelete(Entity * e);//do not call this yourself, call Entiy.requestDestroy instead

protected:
	//overriding giveInteractionManagers() enables the default implementation of register[..]InteractionCheck(). You can instead provide them yourself if you cannot provide giveInteractionManagers().
	virtual void giveInteractionManagers(Entity * e,std::vector<InteractionManager *> * managers,TickServiceProvider * tsp);
public:
	template<typename SymIF>
	virtual void registerSymmetricInteractionCheck(SymIF * me,Entity* e, float seconds,TickServiceProvider* tsp);
};

template<typename SymIF>
inline void IWorld::registerSymmetricInteractionCheck(SymIF* me, Entity* e,float seconds, TickServiceProvider* tsp)
{
	std::vector<InteractionManager *> * vec = tsp->getInterManVector();
	tsp->getChunkManager()->giveInteractionManagers(e,vec,tsp);
	int size=vec->size();
	if(size<=0) WarnErrReporter::notInitializedErr("no chunks found in interManVec: uninitialized chunk?");
	for(int i=0;i<size;i++)
	{
		(*vec)[i]->push.registerInteractionCheck(this,e,seconds,tsp);
	}
}

#endif /* SRC_IWORLD_H_ */
