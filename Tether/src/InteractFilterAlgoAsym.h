/*
 * InteractFilterAlgoAsym.h
 *
 *  Created on:	Nov 6, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTFILTERALGOASYM_H_
#define SRC_INTERACTFILTERALGOASYM_H_

#include "Entity.h"
#include "TickServiceProvider.h"

template<typename MasterIF,typename SlaveIF>
class InteractFilterAlgoAsym
{
public:
	virtual ~InteractFilterAlgoAsym();
	virtual void doPrecalcs();//called before any individual interact requests of the current tick
	virtual void reset();//called after all individual interact requests of the current tick
	virtual void doChecks(MasterIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp)=0;
	virtual void doChecks(SlaveIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp)=0;
};


template<typename MasterIF, typename SlaveIF>
inline InteractFilterAlgoAsym<MasterIF, SlaveIF>::~InteractFilterAlgoAsym()
{}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterAlgoAsym<MasterIF, SlaveIF>::doPrecalcs()
{}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterAlgoAsym<MasterIF, SlaveIF>::reset()
{}

#endif /* SRC_INTERACTFILTERALGOASYM_H_ */
