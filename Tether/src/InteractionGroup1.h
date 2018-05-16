/*
 * InteractionGroup1.h
 *
 *  Created on:	May 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_INTERACTIONGROUP1_H_
#define SRC_INTERACTIONGROUP1_H_
class Entity;
#include "AABB.h"
#include "Entity.h"
#include "TickServiceProvider.h"
#include "DualPointer.h"
#include <vector>

template<typename PhysicsIF>
class InteractionGroup1
{
public:
	std::vector<DualPointer<PhysicsIF>> registered;
	void registerCollisionCheck(PhysicsIF * pIF,Entity * e, float time,TickServiceProvider* tsp);
	void registerCollisionCheck(DualPointer<PhysicsIF>, float time,TickServiceProvider* tsp);
	void check(DualPointer<PhysicsIF> e,DualPointer<PhysicsIF> r,float time,TickServiceProvider* tsp);
	void reset();

	InteractionGroup1();
	~InteractionGroup1();
};

template<typename PhysicsIF>
inline void InteractionGroup1<PhysicsIF>::registerCollisionCheck(DualPointer<PhysicsIF> e, float time, TickServiceProvider* tsp)
{
	int size=registered.size();
	for(int i=0;i<size;i++)
	{
		check(e,registered[i],time,tsp);
		AABB::checkCounter++;
	}
	registered.push_back(e);
}

template<typename PhysicsIF>
inline void InteractionGroup1<PhysicsIF>::check(DualPointer<PhysicsIF> e,DualPointer<PhysicsIF> r, float time, TickServiceProvider* tsp)
{
	if(e.e->bb.doesIntersect(r.e->bb))//doAABBcheck(r.e,time,tsp);
	{
		e.pIF->interact(r.pIF);
	}
}

template<typename PhysicsIF>
inline void InteractionGroup1<PhysicsIF>::reset()
{
	registered.clear();
}

template<typename PhysicsIF>
inline InteractionGroup1<PhysicsIF>::InteractionGroup1()
{}

template<typename PhysicsIF>
inline void InteractionGroup1<PhysicsIF>::registerCollisionCheck(PhysicsIF* pIF,Entity* e, float time, TickServiceProvider* tsp)
{
	registerCollisionCheck(DualPointer<PhysicsIF>(pIF,e),time,tsp);
}

template<typename PhysicsIF>
inline InteractionGroup1<PhysicsIF>::~InteractionGroup1()
{}

#endif /* SRC_INTERACTIONGROUP1_H_ */
