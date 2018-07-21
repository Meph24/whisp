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
	void registerInteractionCheck(PhysicsIF * pIF,Entity * e, float time,TickServiceProvider* tsp);
	void registerInteractionCheck(DualPointer<PhysicsIF>, float time,TickServiceProvider* tsp);
	void check(DualPointer<PhysicsIF> e,DualPointer<PhysicsIF> r,float time,TickServiceProvider* tsp);
	void reset();

	InteractionGroup1();
	~InteractionGroup1();
};

template<typename PhysicsIF>
inline void InteractionGroup1<PhysicsIF>::registerInteractionCheck(DualPointer<PhysicsIF> e, float time, TickServiceProvider* tsp)
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
	//double code starts here, see InteractionGroup2
	if(tsp->tickID!=e.e->lastTickID)
	{
		e.e->reset();
		e.e->lastTickID=tsp->tickID;
	}
	if(tsp->tickID!=r.e->lastTickID)
	{
		r.e->reset();
		r.e->lastTickID=tsp->tickID;
	}
	if(!e.e->bb.doesIntersect(r.e->bb)) return;
	AABB::intersectionCounter++;
	if(e.e->multichunk&&r.e->multichunk)
	{
		int size=e.e->alreadyChecked.size();
		for(int i=0;i<size;i++)
		{
			if((void *)r.pIF==e.e->alreadyChecked[i]) return;
		}
		e.e->alreadyChecked.push_back((void *)r.pIF);
		r.e->alreadyChecked.push_back((void *)e.pIF);
	}
	//double code ends here
	e.pIF->interact(e.e,r,time,tsp);
}

template<typename PhysicsIF>
inline void InteractionGroup1<PhysicsIF>::reset()
{
	registered.clear();
}

template<typename PhysicsIF>
inline void InteractionGroup1<PhysicsIF>::registerInteractionCheck(PhysicsIF* pIF,Entity* e, float time, TickServiceProvider* tsp)
{
	registerInteractionCheck(DualPointer<PhysicsIF>(e,pIF),time,tsp);
}

template<typename PhysicsIF>
inline InteractionGroup1<PhysicsIF>::InteractionGroup1()
{}

template<typename PhysicsIF>
inline InteractionGroup1<PhysicsIF>::~InteractionGroup1()
{}

#endif /* SRC_INTERACTIONGROUP1_H_ */
