/*
 * FilterBoxSortSym.h
 *
 *  Created on:	29.03.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_FILTERBOXSORTSYM_H_
#define SRC_FILTERBOXSORTSYM_H_

#include "InteractFilterAlgoSym.h"
#include "BoxSort.h"
#include "PerformanceMeter.h"


template<typename PhysicsIF>
class FilterBoxSortSym: public InteractFilterAlgoSym<PhysicsIF>
{
	std::vector<InteractFilterEntry<PhysicsIF>> registered;
	BoxSort<PhysicsIF> b;
public:
	PerformanceMeter pm;
	PerformanceMeter::SingleTimer timerOutside;
	PerformanceMeter::SingleTimer timerBuildTree;
	PerformanceMeter::SingleTimer timerRunQueries;
	FilterBoxSortSym();
	virtual ~FilterBoxSortSym();

	void reset();//called between tick rounds for data cleanup
	void doChecks(PhysicsIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void evaluationPhase(TickServiceProvider& tsp);
};

template<typename PhysicsIF>
inline FilterBoxSortSym<PhysicsIF>::FilterBoxSortSym():
b(registered),pm(10s,1s)
{
	timerOutside=pm.createTimestep("outside Boxsort");
	timerOutside.setAsRoundtripMarker("Boxsort Roundtrip");
	timerBuildTree=pm.createTimestep("timerBuildTree");
	timerRunQueries=pm.createTimestep("timerRunQueries");
}

template<typename PhysicsIF>
inline FilterBoxSortSym<PhysicsIF>::~FilterBoxSortSym()
{
}

template<typename PhysicsIF>
inline void FilterBoxSortSym<PhysicsIF>::reset()
{
	registered.clear();
}

template<typename PhysicsIF>
inline void FilterBoxSortSym<PhysicsIF>::doChecks(PhysicsIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	registered.push_back(InteractFilterEntry<PhysicsIF>(me,meAsEntity,time));
}
#include <cstdlib>
template<typename PhysicsIF>
inline void FilterBoxSortSym<PhysicsIF>::evaluationPhase(TickServiceProvider& tsp)
{
	if(registered.empty()) return;
	//timerOutside.registerTime();
	b.buildTree(registered.size(),this->verbose);
	registered.push_back(*(b.root->e));//gets deleted in algorithm, so it must be restored
	//timerBuildTree.registerTime();
	for(InteractFilterEntry<PhysicsIF> fe: registered)
	{
		b.query(fe,tsp);
	}
	//timerRunQueries.registerTime();
	//std::cout<<"Boxsort time BuildTree:"<<timerBuildTree.getData().getAVG()<<std::endl;
	//std::cout<<"Boxsort time RunQueries:"<<timerRunQueries.getData().getAVG()<<std::endl;
}

#endif /* SRC_FILTERBOXSORTSYM_H_ */
