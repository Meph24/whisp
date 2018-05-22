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
#include "AABB.h"
#include "Entity.h"
#include "TickServiceProvider.h"
#include "DualPointer.h"
#include <vector>

template<typename MasterIF,typename SlaveIF>
class InteractionGroup2
{
public:
	std::vector<DualPointer<MasterIF>> firstCalled;
	std::vector<DualPointer<SlaveIF>> secondCalled;
	void registerInteractionCheck(MasterIF * pIF,Entity * e, float time,TickServiceProvider* tsp);
	void registerInteractionCheck(DualPointer<MasterIF> e, float time,TickServiceProvider* tsp);
	void registerInteractionCheck(SlaveIF * pIF,Entity * e, float time,TickServiceProvider* tsp);
	void registerInteractionCheck(DualPointer<SlaveIF> e, float time,TickServiceProvider* tsp);
	void check(DualPointer<MasterIF> f,DualPointer<SlaveIF> s,float time,TickServiceProvider* tsp);
	void reset();
	InteractionGroup2();
	~InteractionGroup2();
};

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(MasterIF* pIF, Entity* e, float time, TickServiceProvider* tsp)
{
	registerInteractionCheck(DualPointer<MasterIF>(pIF,e),time,tsp);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(DualPointer<MasterIF> e, float time, TickServiceProvider* tsp)
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
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(SlaveIF* pIF, Entity* e, float time, TickServiceProvider* tsp)
{
	registerInteractionCheck(DualPointer<SlaveIF>(pIF,e),time,tsp);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::registerInteractionCheck(DualPointer<SlaveIF> e, float time, TickServiceProvider* tsp)
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
inline void InteractionGroup2<MasterIF, SlaveIF>::check(DualPointer<MasterIF> f, DualPointer<SlaveIF> s, float time,TickServiceProvider* tsp)
{
	if(f.e->bb.doesIntersect(s.e->bb))
	{
		f.pIF->interact(f.e,s,time,tsp);
		//s.pIF->interact(s.e,f,time,tsp);
	}
}

template<typename MasterIF, typename SlaveIF>
inline void InteractionGroup2<MasterIF, SlaveIF>::reset()
{
	firstCalled.clear();
	secondCalled.clear();
}

template<typename MasterIF, typename SlaveIF>
inline InteractionGroup2<MasterIF, SlaveIF>::InteractionGroup2()
{}

template<typename MasterIF, typename SlaveIF>
inline InteractionGroup2<MasterIF, SlaveIF>::~InteractionGroup2()
{}

#endif /* SRC_INTERACTIONGROUP2_H_ */
