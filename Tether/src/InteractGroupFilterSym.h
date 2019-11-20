/*
 * InteractGroupFilterSym.h
 *
 *  Created on:	Nov 6, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTGROUPFILTERSYM_H_
#define SRC_INTERACTGROUPFILTERSYM_H_

#include "InteractFilterAlgoSym.h"

template<typename PhysicsIF>
class InteractGroupFilterSym: public InteractFilterAlgoSym<PhysicsIF>
{
protected:

public:
	InteractGroupFilterSym();
	virtual ~InteractGroupFilterSym();

};

#endif /* SRC_INTERACTGROUPFILTERSYM_H_ */
