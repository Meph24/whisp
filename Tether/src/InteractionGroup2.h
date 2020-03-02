/*
 * InteractionGroup2.h
 *
 *  Created on:	May 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_INTERACTIONGROUP2_H_
#define SRC_INTERACTIONGROUP2_H_

class Entity;
class TickServiceProvider;

#include "AABB.h"
#include "DualPointer.h"
#include "Projectile.h"
#include "Entity.h"

#include <vector>

template<typename MasterIF,typename SlaveIF>
class InteractionGroup2
{
	void * filterAlgo;
	bool multichunkInitValue;
public:
	std::vector<DualPointer<MasterIF>> firstCalled;
	std::vector<DualPointer<SlaveIF>> secondCalled;
	void registerInteractionCheck(MasterIF * pIF,Entity * e, float time,TickServiceProvider& tsp);
	void registerInteractionCheck(DualPointer<MasterIF> e, float time,TickServiceProvider& tsp);
	void registerInteractionCheck(SlaveIF * pIF,Entity * e, float time,TickServiceProvider& tsp);
	void registerInteractionCheck(DualPointer<SlaveIF> e, float time,TickServiceProvider& tsp);
	void check(DualPointer<MasterIF> f,DualPointer<SlaveIF> s,float time,TickServiceProvider& tsp);
	void reset();
	InteractionGroup2(void * FilterAlgo,bool MultichunkInitValue=false);
	~InteractionGroup2();
};

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(MasterIF* pIF, Entity* e, float time, TickServiceProvider& tsp)
{
	registerInteractionCheck(DualPointer<MasterIF>(e,pIF),time,tsp);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(DualPointer<MasterIF> e, float time, TickServiceProvider& tsp)
{
	int size=secondCalled.size();
	for(int i=0;i<size;i++)
	{
		check(e,secondCalled[i],time,tsp);
		AABB::checkCounter++;
	}
	firstCalled.push_back(e);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(SlaveIF* pIF, Entity* e, float time, TickServiceProvider& tsp)
{
	registerInteractionCheck(DualPointer<SlaveIF>(e,pIF),time,tsp);
}


template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(DualPointer<SlaveIF> e, float time, TickServiceProvider& tsp)
{
	int size=firstCalled.size();
	for(int i=0;i<size;i++)
	{
		check(firstCalled[i],e,time,tsp);
		AABB::checkCounter++;
	}
	secondCalled.push_back(e);
}


template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::check(DualPointer<MasterIF> f, DualPointer<SlaveIF> s, float time,TickServiceProvider& tsp)
{
	//double code starts here, see InteractionGroup1
	//TODO simplify for InteractionGroup2
	if(tsp.tickID!=f.e->lastTickID)
	{
		f.e->prepareForPartitionAlgo(filterAlgo,multichunkInitValue);//->reset();
		f.e->lastTickID=tsp.tickID;
	}
	if(tsp.tickID!=s.e->lastTickID)
	{
		s.e->prepareForPartitionAlgo(filterAlgo,multichunkInitValue);
		s.e->lastTickID=tsp.tickID;
	}
	if(!f.e->bb.doesIntersect(s.e->bb)) return;
	AABB::intersectionCounter++;
	if(f.e->multichunk&&s.e->multichunk)
	{
		int size=f.e->alreadyChecked.size();
		for(int i=0;i<size;i++)
		{
			if((void *)s.pIF==f.e->alreadyChecked[i]) return;
		}
		f.e->alreadyChecked.push_back((void *)s.pIF);
		s.e->alreadyChecked.push_back((void *)f.pIF);
	}
	//double code ends here
	f.pIF->interact(f.e,s,time,tsp);
	//s.pIF->interact(s.e,f,time,tsp);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::reset()
{
	firstCalled.clear();
	secondCalled.clear();
}

template<typename MasterIF, typename SlaveIF>
inline InteractionGroup2<MasterIF, SlaveIF>::InteractionGroup2(void * FilterAlgo,bool MultichunkInitValue):
filterAlgo(FilterAlgo),multichunkInitValue(MultichunkInitValue)
{}

template<typename MasterIF, typename SlaveIF>
inline InteractionGroup2<MasterIF, SlaveIF>::~InteractionGroup2()
{}

#endif /* SRC_INTERACTIONGROUP2_H_ */
