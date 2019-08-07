/*
 * Pushable.h
 *
 *  Created on:	May 3, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_PUSHABLE_H_
#define SRC_PUSHABLE_H_
#include "Spacevec.h"
#include "DualPointer.h"
class Entity;
class TickServiceProvider;
class Pushable
{
public:
	float pushRadius=0;
	float pushForce=0;
	void registerPushCheck(Entity * e,float seconds,TickServiceProvider * tsp);
	void interact(Entity * self,DualPointer<Pushable> other, float time, TickServiceProvider* tsp);
	virtual void push(spacevec amount, TickServiceProvider* tsp)=0;
	virtual ~Pushable();
};

#include "TickServiceProvider.h"
#include "Entity.h"

#endif /* SRC_PUSHABLE_H_ */
