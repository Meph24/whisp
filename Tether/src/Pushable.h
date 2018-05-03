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
class Pushable
{
public:
	float pushRadius=0;
	float pushForce=0;
	virtual void push(spacevec amount)=0;
	virtual spacevec getPos()=0;
	virtual ~Pushable();
};

#endif /* SRC_PUSHABLE_H_ */
