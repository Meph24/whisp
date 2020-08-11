/*
 * FilterHashSym.h
 *
 *  Created on:	14.04.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_FILTERHASHSYM_H_
#define SRC_FILTERHASHSYM_H_

#include "InteractFilterAlgoSym.h"
#include "FilterDuplicateBool.h"
#include "chunkCooHash.h"

#include <unordered_map>
#include <vector>
#include <iostream>



template<typename PhysicsIF>
class FilterHashSym: public InteractFilterAlgoSym<PhysicsIF>
{
	std::unordered_map<chunkCoo,std::vector<InteractFilterEntry<PhysicsIF>>,chunkCooHash> table;
	std::unordered_map<Entity *,FilterDuplicateBool> duplicates;
public:
	unsigned long long registeredNum=0;
	unsigned long long potentialChecks=0;
	unsigned long long checks=0;
	unsigned long long intersections=0;

	FilterHashSym();
	virtual ~FilterHashSym();

	void reset();//called between tick rounds for data cleanup
	void doChecks(PhysicsIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);

	void check(InteractFilterEntry<PhysicsIF>& first, InteractFilterEntry<PhysicsIF>& second,TickServiceProvider& tsp);
};

template<typename PhysicsIF>
inline FilterHashSym<PhysicsIF>::FilterHashSym()
{

}

template<typename PhysicsIF>
inline FilterHashSym<PhysicsIF>::~FilterHashSym()
{
}

template<typename PhysicsIF>
inline void FilterHashSym<PhysicsIF>::reset()
{

	if(this->verbose) std::cout<<std::endl<<"hash: "<<std::endl;
	if(this->verbose) std::cout<<"occupied chunks (last time):"<<table.size()<<std::endl;
	table.clear();
	if(this->verbose) std::cout<<"registered entities:"<<registeredNum<<std::endl;
	if(this->verbose) std::cout<<"potential checks:"<<potentialChecks<<std::endl;
	if(this->verbose) std::cout<<"checks:"<<checks<<std::endl;
	if(this->verbose) std::cout<<"intersections:"<<intersections<<std::endl;
	checks=0;
	intersections=0;
	potentialChecks=0;
	registeredNum=0;
}

template<typename PhysicsIF>
inline void FilterHashSym<PhysicsIF>::doChecks(PhysicsIF* me, Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	InteractFilterEntry<PhysicsIF> entry(me,meAsEntity,time);
	bool isMultichunk=entry.e.e->bb.isMultichunk();
	chunkCoo low(entry.e.e->bb.low);
	potentialChecks+=registeredNum;
	if(!isMultichunk)
	{
		std::vector<InteractFilterEntry<PhysicsIF>>& vec=table[low];
		for(InteractFilterEntry<PhysicsIF>& elem: vec)
		{
			check(entry,elem,tsp);
		}
		vec.push_back(entry);
		registeredNum++;
	}
	else
	{
		chunkCoo high(entry.e.e->bb.high);
		assert((high.z-low.z)<1000);
		assert((high.y-low.y)<1000);
		assert((high.x-low.x)<1000);
		for(gridInt z=low.z;z!=high.z+1;z++)
		{
			for(gridInt y=low.y;y!=high.y+1;y++)
			{
				for(gridInt x=low.x;x!=high.x+1;x++)
				{
					std::vector<InteractFilterEntry<PhysicsIF>>& vec=table[chunkCoo(x,y,z)];
					for(InteractFilterEntry<PhysicsIF>& elem: vec)
					{
						bool otherMultichunk=elem.e.e->bb.isMultichunk();
						if(otherMultichunk)
						{
							if(!duplicates[elem.e.e].alreadyChecked)
							{
								check(entry,elem,tsp);
								duplicates[elem.e.e].alreadyChecked=true;
							}
						}
						else
						{
							check(entry,elem,tsp);
						}
					}
					vec.push_back(entry);
					registeredNum++;
				}
			}
		}
		duplicates.clear();
	}
}

template<typename PhysicsIF>
inline void FilterHashSym<PhysicsIF>::check(InteractFilterEntry<PhysicsIF>& first, InteractFilterEntry<PhysicsIF>& second,TickServiceProvider& tsp)
{
	checks++;
	if(!(first.e.e->bb.doesIntersect(second.e.e->bb))) return;
	intersections++;
	first.e.pIF->interact(first.e.e,second.e,first.time,tsp);//TODO rethink "time" parameter
}

#endif /* SRC_FILTERHASHSYM_H_ */
