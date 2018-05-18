/*
 * InteractionGroup2.h
 *
 *  Created on:	May 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_INTERACTIONGROUP2_H_
#define SRC_INTERACTIONGROUP2_H_
class Entity;
#include "AABB.h"
#include "Entity.h"
#include "TickServiceProvider.h"
#include "DualPointer.h"
#include <vector>

template<typename FirstIF,typename SecondIF>
class InteractionGroup2
{
public:
	std::vector<DualPointer<FirstIF>> firstCalled;
	std::vector<DualPointer<SecondIF>> secondCalled;
	void registerInteractionCheck(FirstIF * pIF,Entity * e, float time,TickServiceProvider* tsp);
	void registerInteractionCheck(DualPointer<FirstIF>, float time,TickServiceProvider* tsp);
	void registerInteractionCheck(SecondIF * pIF,Entity * e, float time,TickServiceProvider* tsp);
	void registerInteractionCheck(DualPointer<SecondIF>, float time,TickServiceProvider* tsp);
	void check(DualPointer<firstCalled> f,DualPointer<SecondIF> s,float time,TickServiceProvider* tsp);
	void reset();
	InteractionGroup2();
	~InteractionGroup2();
};

template<typename FirstIF, typename SecondIF>
inline void InteractionGroup2<FirstIF, SecondIF>::registerInteractionCheck(FirstIF* pIF, Entity* e, float time, TickServiceProvider* tsp)
{
}

template<typename FirstIF, typename SecondIF>
inline void InteractionGroup2<FirstIF, SecondIF>::registerInteractionCheck(DualPointer<FirstIF> dualPointer, float time, TickServiceProvider* tsp)
{
}

template<typename FirstIF, typename SecondIF>
inline void InteractionGroup2<FirstIF, SecondIF>::registerInteractionCheck(SecondIF* pIF, Entity* e, float time, TickServiceProvider* tsp)
{
}

template<typename FirstIF, typename SecondIF>
inline void InteractionGroup2<FirstIF, SecondIF>::registerInteractionCheck(DualPointer<SecondIF> dualPointer, float time, TickServiceProvider* tsp)
{
}

template<typename FirstIF, typename SecondIF>
inline void InteractionGroup2<FirstIF, SecondIF>::check(DualPointer<firstCalled> f, DualPointer<SecondIF> s, float time,
		TickServiceProvider* tsp)
{
}

template<typename FirstIF, typename SecondIF>
inline void InteractionGroup2<FirstIF, SecondIF>::reset()
{
}

template<typename FirstIF, typename SecondIF>
inline InteractionGroup2<FirstIF, SecondIF>::InteractionGroup2()
{
}

template<typename FirstIF, typename SecondIF>
inline InteractionGroup2<FirstIF, SecondIF>::~InteractionGroup2()
{
}

#endif /* SRC_INTERACTIONGROUP2_H_ */
