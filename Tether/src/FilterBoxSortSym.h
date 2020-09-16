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


template<typename PhysicsIF>
class FilterBoxSortSym: public InteractFilterAlgoSym<PhysicsIF>
{
	std::vector<InteractFilterEntry<PhysicsIF>> registered;
	BoxSort<PhysicsIF> b;
public:
	FilterBoxSortSym();
	virtual ~FilterBoxSortSym();

	void reset();//called between tick rounds for data cleanup
	void doChecks(PhysicsIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void evaluationPhase(TickServiceProvider& tsp);
};

template<typename PhysicsIF>
inline FilterBoxSortSym<PhysicsIF>::FilterBoxSortSym():
b(registered)
{
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
	b.buildTree(registered.size(),this->verbose);
	for(InteractFilterEntry<PhysicsIF> fe: registered)
	{
		b.query(fe,tsp);
	}
}

#endif /* SRC_FILTERBOXSORTSYM_H_ */
