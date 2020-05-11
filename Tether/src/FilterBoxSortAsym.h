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

#define MASTER_ONLY INFINITY
#define SLAVE_ONLY 0


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
	float treeDecisionMasterMultiplier;
	//the number of registered master entities is multiplied with this value in order to determine
	//which collection of entities the tree is built from
	//e.g. more entities of master type -> tree built from master entities
	//you can use MASTER_ONLY and SLAVE_ONLY to disable dynamic switching
	FilterBoxSortAsym(float TreeDecisionMasterMultiplier=1);
	virtual ~FilterBoxSortAsym();

	void reset();//called between tick rounds for data cleanup
	void doChecks(MasterIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void doChecks(SlaveIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void evaluationPhase(TickServiceProvider& tsp);
};

template<typename MasterIF, typename SlaveIF>
inline FilterBoxSortAsym<MasterIF, SlaveIF>::FilterBoxSortAsym(float TreeDecisionMasterMultiplier):
registeredMaster(),registeredSlave(),bm(registeredMaster),bs(registeredSlave),treeDecisionMasterMultiplier(TreeDecisionMasterMultiplier)
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
	float masterValue=registeredMaster.size()*treeDecisionMasterMultiplier;
	float slaveValue=registeredSlave.size();
	if(masterValue>slaveValue)
	{
		if(registeredSlave.empty()) return;
		bm.buildTree(registeredSlave.size());
		for(InteractFilterEntry<SlaveIF> fe: registeredSlave)
		{
			bm.template query<SlaveIF,false>(fe,tsp);
		}
	}
	else
	{
		if(registeredMaster.empty()) return;
		bs.buildTree(registeredMaster.size());
		for(InteractFilterEntry<MasterIF> fe: registeredMaster)
		{
			bs.template query<MasterIF,true>(fe,tsp);
		}
	}
}

#endif /* SRC_FILTERBOXSORTASYM_H_ */
