/*
 * InteractFilterNoneS.h
 *
 *  Created on:	06.10.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTFILTERNONES_H_
#define SRC_INTERACTFILTERNONES_H_

#include "InteractFilterAlgoSym.h"
#include "DualPointer.h"

template<typename PhysicsIF>
class InteractFilterNoneS: public InteractFilterAlgoSym<PhysicsIF>
{
	std::vector<DualPointer<PhysicsIF>> registered;
public:
	InteractFilterNoneS();
	virtual ~InteractFilterNoneS();
	virtual void reset();//called between tick rounds for data cleanup
	virtual void doChecks(PhysicsIF * me,Entity * meAsEntity,float time,TickServiceProvider& tsp);
	void check(DualPointer<PhysicsIF> e,DualPointer<PhysicsIF> r,float time,TickServiceProvider& tsp);
};

template<typename PhysicsIF>
inline InteractFilterNoneS<PhysicsIF>::InteractFilterNoneS()
{
}

template<typename PhysicsIF>
inline InteractFilterNoneS<PhysicsIF>::~InteractFilterNoneS()
{
}

template<typename PhysicsIF>
inline void InteractFilterNoneS<PhysicsIF>::reset()
{
	registered.clear();
}

template<typename PhysicsIF>
inline void InteractFilterNoneS<PhysicsIF>::doChecks(PhysicsIF* me,Entity* meAsEntity, float time, TickServiceProvider& tsp)
{
	DualPointer<PhysicsIF> meDP(meAsEntity,me);
	for(DualPointer<PhysicsIF> elem: registered)
	{
		check(meDP,elem,time,tsp);
	}
	registered.push_back(meDP);
}

template<typename PhysicsIF>
inline void InteractFilterNoneS<PhysicsIF>::check(DualPointer<PhysicsIF> e,DualPointer<PhysicsIF> r, float time, TickServiceProvider& tsp)
{
	e.pIF->interact(e.e,r,time,tsp);
}

#endif /* SRC_INTERACTFILTERNONES_H_ */
