/*
 * InteractFilterEntry.h
 *
 *  Created on:	29.03.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_INTERACTFILTERENTRY_H_
#define SRC_INTERACTFILTERENTRY_H_

#include "DualPointer.h"

template<typename PhysicsIF>
class InteractFilterEntry
{
public:
	DualPointer<PhysicsIF> e;
	float time;
	InteractFilterEntry(PhysicsIF* me,Entity* meAsEntity, float Time);
	virtual ~InteractFilterEntry();
};

template<typename PhysicsIF>
inline InteractFilterEntry<PhysicsIF>::InteractFilterEntry(PhysicsIF* me,Entity* meAsEntity, float Time):
e(meAsEntity,me),time(Time)
{
}


template<typename PhysicsIF>
inline InteractFilterEntry<PhysicsIF>::~InteractFilterEntry()
{
	//pointers are not owned, so no deletions needed
}

#endif /* SRC_INTERACTFILTERENTRY_H_ */
