/*
 * InteractFilterAlgoAsym.h
 *
 *  Created on:	Nov 6, 2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_INTERACTFILTERALGOASYM_H_
#define SRC_INTERACTFILTERALGOASYM_H_

#include "Entity.h"
#include "TickServiceProvider.h"

template<typename MasterIF,typename SlaveIF>
class InteractFilterAlgoAsym
{
public:
	bool verbose=true;
	InteractFilterAlgoAsym();
	virtual ~InteractFilterAlgoAsym();
	virtual void doPrecalcs(TickServiceProvider& tsp);//called before any individual interact requests of the current tick
	virtual void reset();//called between tick rounds for data cleanup
	virtual void doChecks(MasterIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp)=0;
	virtual void doChecks(SlaveIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp)=0;
	virtual void evaluationPhase(TickServiceProvider& tsp);//called after every tick. Used by algorithms that first only collect, then execute later on the full set of participants
};


template<typename MasterIF, typename SlaveIF>
inline InteractFilterAlgoAsym<MasterIF, SlaveIF>::~InteractFilterAlgoAsym()
{}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterAlgoAsym<MasterIF, SlaveIF>::doPrecalcs(TickServiceProvider& tsp)
{
	//default: unused
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterAlgoAsym<MasterIF, SlaveIF>::reset()
{
	//default: unused
}

template<typename MasterIF, typename SlaveIF>
inline InteractFilterAlgoAsym<MasterIF, SlaveIF>::InteractFilterAlgoAsym()
{
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterAlgoAsym<MasterIF, SlaveIF>::evaluationPhase(TickServiceProvider& tsp)
{
	//default: unused
}

#endif /* SRC_INTERACTFILTERALGOASYM_H_ */
