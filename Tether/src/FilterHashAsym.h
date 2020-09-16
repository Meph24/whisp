/*
 * FilterHashAsym.h
 *
 *  Created on:	14.04.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_FILTERHASHASYM_H_
#define SRC_FILTERHASHASYM_H_

#include "InteractFilterAlgoAsym.h"
#include "chunkCooHash.h"
#include "FilterDuplicateBool.h"

#include <unordered_map>
#include <iostream>

template<typename MasterIF,typename SlaveIF>
class FilterHashAsym: public InteractFilterAlgoAsym<MasterIF,SlaveIF>
{
	std::unordered_map<chunkCoo,std::vector<InteractFilterEntry<MasterIF>>,chunkCooHash> tableM;
	std::unordered_map<chunkCoo,std::vector<InteractFilterEntry<SlaveIF>>,chunkCooHash> tableS;
	std::unordered_map<Entity *,FilterDuplicateBool> duplicates;
public:
	unsigned long long registeredNumM=0;
	unsigned long long registeredNumS=0;
	unsigned long long potentialChecks=0;
	unsigned long long checks=0;
	unsigned long long intersections=0;

	FilterHashAsym();
	virtual ~FilterHashAsym();

	void reset();//called between tick rounds for data cleanup
	void doChecks(MasterIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void doChecks(SlaveIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);

	void check(InteractFilterEntry<MasterIF>& first, InteractFilterEntry<SlaveIF>& second,TickServiceProvider& tsp);
};

template<typename MasterIF, typename SlaveIF>
inline FilterHashAsym<MasterIF, SlaveIF>::FilterHashAsym()
{
}

template<typename MasterIF, typename SlaveIF>
inline FilterHashAsym<MasterIF, SlaveIF>::~FilterHashAsym()
{
}

template<typename MasterIF, typename SlaveIF>
inline void FilterHashAsym<MasterIF, SlaveIF>::reset()
{
	if(this->verbose) std::cout<<std::endl<<"hash:"<<std::endl;
	if(this->verbose) std::cout<<"occupied chunks by MasterIF (last time):"<<tableM.size()<<std::endl;
	if(this->verbose) std::cout<<"occupied chunks by SlaveIF (last time):"<<tableS.size()<<std::endl;
	tableM.clear();
	tableS.clear();
	if(this->verbose) std::cout<<"registered entities (MasterIF):"<<registeredNumM<<std::endl;
	if(this->verbose) std::cout<<"registered entities (SlaveIF):"<<registeredNumS<<std::endl;
	if(this->verbose) std::cout<<"potential checks:"<<potentialChecks<<std::endl;
	if(this->verbose) std::cout<<"checks:"<<checks<<std::endl;
	if(this->verbose) std::cout<<"intersections:"<<intersections<<std::endl;
	checks=0;
	intersections=0;
	potentialChecks=0;
	registeredNumM=0;
	registeredNumS=0;
}

template<typename MasterIF, typename SlaveIF>
inline void FilterHashAsym<MasterIF, SlaveIF>::doChecks(MasterIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	InteractFilterEntry<MasterIF> entry(me,meAsEntity,time);
	bool isMultichunk=entry.e.e->bb.isMultichunk();
	chunkCoo low(entry.e.e->bb.low);
	potentialChecks+=registeredNumS;
	if(!isMultichunk)
	{
		std::vector<InteractFilterEntry<SlaveIF>>& vec=tableS[low];
		for(InteractFilterEntry<SlaveIF>& elem: vec)
		{
			check(entry,elem,tsp);
		}
		tableM[low].push_back(entry);
		registeredNumM++;
	}
	else
	{
		chunkCoo high(entry.e.e->bb.high);
		assert((high.z-low.z)<1000);
		assert((high.y-low.y)<1000);
		assert((high.x-low.x)<1000);
		bool foundChunk=false;
		for(gridInt z=low.z;z!=high.z+1;z++)
		{
			for(gridInt y=low.y;y!=high.y+1;y++)
			{
				for(gridInt x=low.x;x!=high.x+1;x++)
				{
					std::vector<InteractFilterEntry<SlaveIF>>& vec=tableS[chunkCoo(x,y,z)];
					for(InteractFilterEntry<SlaveIF>& elem: vec)
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
					tableM[chunkCoo(x,y,z)].push_back(entry);
					foundChunk=true;
				}
			}
		}
		if(foundChunk) registeredNumM++;
		duplicates.clear();
	}
}

template<typename MasterIF, typename SlaveIF>
inline void FilterHashAsym<MasterIF, SlaveIF>::doChecks(SlaveIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	InteractFilterEntry<SlaveIF> entry(me,meAsEntity,time);
	bool isMultichunk=entry.e.e->bb.isMultichunk();
	chunkCoo low(entry.e.e->bb.low);
	potentialChecks+=registeredNumS;
	bool foundChunk=false;
	if(!isMultichunk)
	{
		std::vector<InteractFilterEntry<MasterIF>>& vec=tableM[low];
		for(InteractFilterEntry<MasterIF>& elem: vec)
		{
			check(elem,entry,tsp);
		}
		tableS[low].push_back(entry);
		registeredNumS++;
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
					std::vector<InteractFilterEntry<MasterIF>>& vec=tableM[chunkCoo(x,y,z)];
					for(InteractFilterEntry<MasterIF>& elem: vec)
					{
						bool otherMultichunk=elem.e.e->bb.isMultichunk();
						if(otherMultichunk)
						{
							if(!duplicates[elem.e.e].alreadyChecked)
							{
								check(elem,entry,tsp);
								duplicates[elem.e.e].alreadyChecked=true;
							}
						}
						else
						{
							check(elem,entry,tsp);
						}
					}
					tableS[chunkCoo(x,y,z)].push_back(entry);
					foundChunk=true;
				}
			}
		}
		if(foundChunk) registeredNumS++;
		duplicates.clear();
	}
}

template<typename MasterIF, typename SlaveIF>
inline void FilterHashAsym<MasterIF, SlaveIF>::check(InteractFilterEntry<MasterIF>& first,InteractFilterEntry<SlaveIF>& second, TickServiceProvider& tsp)
{
	checks++;
	if(!(first.e.e->bb.doesIntersect(second.e.e->bb))) return;
	intersections++;
	first.e.pIF->interact(first.e.e,second.e,first.time,tsp);//TODO rethink "time" parameter
}

#endif /* SRC_FILTERHASHASYM_H_ */
