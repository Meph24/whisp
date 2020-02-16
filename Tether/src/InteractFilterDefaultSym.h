/*
 * InteractFilterDefaultSym.h
 *
 *  Created on:	Nov 14, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTFILTERDEFAULTSYM_H_
#define SRC_INTERACTFILTERDEFAULTSYM_H_

#include "InteractFilterAlgoSym.h"
#include "DualPointer.h"
#include "InteractionGroup1.h"

#include <vector>

template<typename PhysicsIF>
class InteractFilterDefaultSym: public InteractFilterAlgoSym<PhysicsIF>
{
	InteractionGroup1<PhysicsIF> group;
public:
	InteractFilterDefaultSym();
	virtual ~InteractFilterDefaultSym();
	virtual void reset();//called after all individual interact requests of the current tick
	virtual void doChecks(PhysicsIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
};

template<typename PhysicsIF>
inline InteractFilterDefaultSym<PhysicsIF>::InteractFilterDefaultSym():
group(this)
{}

template<typename PhysicsIF>
inline InteractFilterDefaultSym<PhysicsIF>::~InteractFilterDefaultSym()
{}

template<typename PhysicsIF>
inline void InteractFilterDefaultSym<PhysicsIF>::reset()
{
	group.reset();
}

template<typename PhysicsIF>
inline void InteractFilterDefaultSym<PhysicsIF>::doChecks(PhysicsIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	group.registerInteractionCheck(me,meAsEntity,time,tsp);
}

#endif /* SRC_INTERACTFILTERDEFAULTSYM_H_ */
