/*
 * InteractFilterNoneA.h
 *
 *  Created on:	08.10.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTFILTERNONEA_H_
#define SRC_INTERACTFILTERNONEA_H_

#include "InteractFilterAlgoAsym.h"
#include "DualPointer.h"

template<typename MasterIF,typename SlaveIF>
class InteractFilterNoneA: public InteractFilterAlgoAsym<MasterIF,SlaveIF>
{
	std::vector<DualPointer<MasterIF>> registeredM;
	std::vector<DualPointer<SlaveIF>> registeredS;
public:
	InteractFilterNoneA();
	virtual ~InteractFilterNoneA();

	virtual void reset();//called between tick rounds for data cleanup
	virtual void doChecks(MasterIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	virtual void doChecks(SlaveIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void check(DualPointer<MasterIF> f,DualPointer<SlaveIF> s,float time,TickServiceProvider& tsp);

};

template<typename MasterIF, typename SlaveIF>
inline InteractFilterNoneA<MasterIF, SlaveIF>::InteractFilterNoneA()
{
}

template<typename MasterIF, typename SlaveIF>
inline InteractFilterNoneA<MasterIF, SlaveIF>::~InteractFilterNoneA()
{
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterNoneA<MasterIF, SlaveIF>::reset()
{
	registeredM.clear();
	registeredS.clear();
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterNoneA<MasterIF, SlaveIF>::doChecks(MasterIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	DualPointer<MasterIF> meDP(meAsEntity,me);
	for(DualPointer<SlaveIF> elem: registeredS)
	{
		check(meDP,elem,time,tsp);
	}
	registeredM.push_back(meDP);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterNoneA<MasterIF, SlaveIF>::doChecks(SlaveIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	DualPointer<SlaveIF> meDP(meAsEntity,me);
	for(DualPointer<MasterIF> elem: registeredM)
	{
		check(elem,meDP,time,tsp);
	}
	registeredS.push_back(meDP);
}

template<typename MasterIF, typename SlaveIF>
inline void InteractFilterNoneA<MasterIF, SlaveIF>::check(DualPointer<MasterIF> f, DualPointer<SlaveIF> s, float time,TickServiceProvider& tsp)
{
	f.pIF->interact(f.e,s,time,tsp);
}

#endif /* SRC_INTERACTFILTERNONEA_H_ */
