/*
 * InteractFilterAlgoSym.h
 *
 *  Created on:	Nov 6, 2019
 *      Author:	HL65536
 *     Version:	1.0
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
	virtual void doPrecalcs();//called before any individual interact requests of the current tick
	virtual void reset();//called after all individual interact requests of the current tick
	virtual void doChecks(PhysicsIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp)=0;
};

template<typename PhysicsIF>
inline InteractFilterAlgoSym<PhysicsIF>::~InteractFilterAlgoSym()
{}

template<typename PhysicsIF>
inline void InteractFilterAlgoSym<PhysicsIF>::doPrecalcs()
{}

template<typename PhysicsIF>
inline void InteractFilterAlgoSym<PhysicsIF>::reset()
{}

#endif /* SRC_INTERACTFILTERALGOSYM_H_ */
