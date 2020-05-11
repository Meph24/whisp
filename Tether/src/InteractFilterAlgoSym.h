/*
 * InteractFilterAlgoSym.h
 *
 *  Created on:	Nov 6, 2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_INTERACTFILTERALGOSYM_H_
#define SRC_INTERACTFILTERALGOSYM_H_

#include "Entity.h"
#include "TickServiceProvider.h"


template<typename PhysicsIF>
class InteractFilterAlgoSym
{
public:
	virtual ~InteractFilterAlgoSym();
	virtual void doPrecalcs(TickServiceProvider& tsp);//called before any individual interact requests of the current tick
	virtual void reset();//called between tick rounds for data cleanup
	virtual void doChecks(PhysicsIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp)=0;
	virtual void evaluationPhase(TickServiceProvider& tsp );//called after every tick. Used by algorithms that first only collect, then execute later on the full set of participants
};

template<typename PhysicsIF>
inline InteractFilterAlgoSym<PhysicsIF>::~InteractFilterAlgoSym()
{}

template<typename PhysicsIF>
inline void InteractFilterAlgoSym<PhysicsIF>::doPrecalcs(TickServiceProvider& tsp)
{
	//default: unused
}

template<typename PhysicsIF>
inline void InteractFilterAlgoSym<PhysicsIF>::reset()
{
	//default: unused
}

template<typename PhysicsIF>
inline void InteractFilterAlgoSym<PhysicsIF>::evaluationPhase(TickServiceProvider& tsp)
{
	//default: unused
}

#endif /* SRC_INTERACTFILTERALGOSYM_H_ */
