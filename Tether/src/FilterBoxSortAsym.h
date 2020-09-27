/*
 * FilterBoxSortAsym.h
 *
 *  Created on:	31.03.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_FILTERBOXSORTASYM_H_
#define SRC_FILTERBOXSORTASYM_H_

#include <cmath>

enum TreeBuildMode
{
	onlyMaster,
	onlySlave,
	chooseMore,
	chooseLess
};

#include "InteractFilterAlgoAsym.h"
#include "BoxSort.h"

template<typename MasterIF,typename SlaveIF>
class FilterBoxSortAsym: public InteractFilterAlgoAsym<MasterIF,SlaveIF>
{
	std::vector<InteractFilterEntry<MasterIF>> registeredMaster;
	std::vector<InteractFilterEntry<SlaveIF>> registeredSlave;
	BoxSort<MasterIF> bm;
	BoxSort<SlaveIF> bs;
public:
	TreeBuildMode tbm;
	float preferMasterBy;
	//the number of registered master entities is multiplied with this value in order to determine
	//which collection of entities the tree is built from
	//e.g. more entities of master type -> tree built from master entities
	//you can use MASTER_ONLY and SLAVE_ONLY to disable dynamic switching
	FilterBoxSortAsym(TreeBuildMode t,float PreferMasterBy=1);
	virtual ~FilterBoxSortAsym();

	void reset();//called between tick rounds for data cleanup
	void doChecks(MasterIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void doChecks(SlaveIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void evaluationPhase(TickServiceProvider& tsp);
};

template<typename MasterIF, typename SlaveIF>
inline FilterBoxSortAsym<MasterIF, SlaveIF>::FilterBoxSortAsym(TreeBuildMode t,float PreferMasterBy):
registeredMaster(),registeredSlave(),bm(registeredMaster),bs(registeredSlave),tbm(t),preferMasterBy(PreferMasterBy)
{
}

template<typename MasterIF, typename SlaveIF>
inline FilterBoxSortAsym<MasterIF, SlaveIF>::~FilterBoxSortAsym()
{
}

template<typename MasterIF, typename SlaveIF>
inline void FilterBoxSortAsym<MasterIF, SlaveIF>::reset()
{
	registeredMaster.clear();
	registeredSlave.clear();
}

template<typename MasterIF, typename SlaveIF>
inline void FilterBoxSortAsym<MasterIF, SlaveIF>::doChecks(MasterIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	registeredMaster.push_back(InteractFilterEntry<MasterIF>(me,meAsEntity,time));
}

template<typename MasterIF, typename SlaveIF>
inline void FilterBoxSortAsym<MasterIF, SlaveIF>::doChecks(SlaveIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	registeredSlave.push_back(InteractFilterEntry<SlaveIF>(me,meAsEntity,time));
}

template<typename MasterIF, typename SlaveIF>
inline void FilterBoxSortAsym<MasterIF, SlaveIF>::evaluationPhase(TickServiceProvider& tsp)
{
	if(registeredMaster.empty()) return;
	if(registeredSlave.empty()) return;

	bool chooseMaster=true;
	switch(tbm)
	{
	case onlyMaster: chooseMaster=true; break;
	case onlySlave: chooseMaster=false; break;
	case chooseLess:
		{float masterValue=registeredMaster.size();
		float slaveValue=registeredSlave.size();
		chooseMaster=masterValue<slaveValue*preferMasterBy;}
		break;
	case chooseMore:
		{float masterValue=registeredMaster.size();
		float slaveValue=registeredSlave.size();
		chooseMaster=masterValue*preferMasterBy>slaveValue;}
		break;

	}
	if(chooseMaster)
	{
		if(registeredSlave.empty()) return;
		bm.buildTree(registeredSlave.size(),this->verbose);//the guy that thought it was a good idea to write in the C++ standard that just "verbose" would lead to a "was not declared in this scope" should face a life sentence for crimes against humanity
		for(InteractFilterEntry<SlaveIF> fe: registeredSlave)
		{
			bm.template query<SlaveIF,false>(fe,tsp);
		}
	}
	else
	{
		if(registeredMaster.empty()) return;
		bs.buildTree(registeredMaster.size(),this->verbose);
		for(InteractFilterEntry<MasterIF> fe: registeredMaster)
		{
			bs.template query<MasterIF,true>(fe,tsp);
		}
	}
}

#endif /* SRC_FILTERBOXSORTASYM_H_ */
