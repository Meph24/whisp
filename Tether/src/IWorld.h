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
#include "Pushable.h"
#include "Hittable.h"
#include "Projectile.h"
#include "Collider.hpp"
#include "BenchAsymMaster.h"
#include "BenchAsymSlave.h"
#include "BenchSym.h"
#include "CreationDestructionListener.h"
#include "Syncable.h"

template<typename PhysicsIF>
class InteractFilterAlgoSym;

template<typename MasterIF,typename SlaveIF>
class InteractFilterAlgoAsym;

#include <vector>

class IWorld: public Tickable, public Drawable, public CreationDestructionListener<Entity>
{
private:

	void initAlgos();

protected:
	std::vector<Entity *> hibernating;

	float gridSize;//sacelen.intpart scale in meters
	std::vector<Entity *> deleteVec;//the entities that should be removed from world
	//TODO addVec

	void resetAlgos(TickServiceProvider& tsp);
public:

	CreationDestructionNotificationHandler<Entity> entityNotif;

	bool verbose;

	InteractFilterAlgoSym<Pushable>* pushAlgo=0;
	InteractFilterAlgoAsym<Projectile,Hittable>* projectileAlgo=0;
	InteractFilterAlgoSym<Collider>* collideAlgo=0;

	InteractFilterAlgoSym<BenchSym>* benchAlgoSym=0;
	InteractFilterAlgoAsym<BenchAsymMaster,BenchAsymSlave>* benchAlgoAsym=0;
	//checklist for adding new interactions:
	//1. add it here
	//2. make sure the value is initialized somewhere
	//3. make sure the value is deleted somewhere
	//4. make sure these are called somewhere: reset(); doPrecalcs(tsp); evaluationPhase(tsp);
	//yes this should be simplified in the future

	IWorld();
	IWorld(float GridSize);
	virtual ~IWorld();

	//only allowed for RELATIVE positions/velocities:
	float toMeters(spacelen l);
	double toMetersD(spacelen l);
	vec3 toMeters(spacevec v);
	spacelen fromMeters(float l) const;
	spacevec fromMeters(const vec3& v) const;

	spacevec toUnitLength(spacevec v);//returns a vector of length 1m in the direction of the passed vector


	virtual void preTick(TickServiceProvider& tsp);
	virtual void finishTick(TickServiceProvider& tsp);//finish main tick phase BEFORE reticks
	virtual void postTick(TickServiceProvider& tsp)=0;//after all ticks and reticks

	//Currently inactive
	virtual void leaveWorld(Entity * e,TickServiceProvider * tsp);//is called if Entity is outside loaded area
	virtual void hibernate(Entity * e);//Entity outside loaded area, but not deleted, does not get ticked
	virtual void wakeHibernating(AABB bb);//awakes hibernating Entities inside bb and tries to insert them into active area
	virtual void wakeHibernating();//awakes all hibernating Entities and tries to insert them into active area
	virtual void wakeHibernating(Entity * e);//awakes this hibernating Entity and tries to insert it into active area, returns 0 if successful, e otherwise

	virtual void requestEntitySpawn(Entity * e)=0;//spawn entity in world, call only once per entity!!! Can fail if not within loaded chunks

	virtual void requestEntityDelete(Entity * e);//do not call this yourself, call Entiy.requestDestroy instead

	virtual void clearEntities()=0;

	virtual void getOwnedSyncables(std::vector<Syncable *> collectHere)=0;
};


#endif /* SRC_IWORLD_H_ */
