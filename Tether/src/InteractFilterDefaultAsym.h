/*
 * InteractFilterDefaultAsym.h
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTFILTERDEFAULTASYM_H_
#define SRC_INTERACTFILTERDEFAULTASYM_H_

#include "InteractFilterAlgoAsym.h"

#include "InteractionGroup2.h"

template<typename MasterIF,typename SlaveIF>
class InteractFilterDefaultAsym: public InteractFilterAlgoAsym<MasterIF,SlaveIF>
{
	InteractionGroup2<MasterIF,SlaveIF> group;
public:
	InteractFilterDefaultAsym();
	virtual ~InteractFilterDefaultAsym();
	virtual void reset();//called after all individual interact requests of the current tick
	virtual void doChecks(MasterIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	virtual void doChecks(SlaveIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
};

template<typename MasterIF, typename SlaveIF>
inline InteractFilterDefaultAsym<MasterIF, SlaveIF>::InteractFilterDefaultAsym():
group(this)
{}

template<typename MasterIF, typename SlaveIF>
inline InteractFilterDefaultAsym<MasterIF, SlaveIF>::~InteractFilterDefaultAsym()
{}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterDefaultAsym<MasterIF, SlaveIF>::reset()
{
	group.reset();
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterDefaultAsym<MasterIF, SlaveIF>::doChecks(MasterIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	group.registerInteractionCheck(me,meAsEntity,time,tsp);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterDefaultAsym<MasterIF, SlaveIF>::doChecks(SlaveIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	group.registerInteractionCheck(me,meAsEntity,time,tsp);
}

#endif /* SRC_INTERACTFILTERDEFAULTASYM_H_ */
