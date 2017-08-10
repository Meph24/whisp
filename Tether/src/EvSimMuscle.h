/*
 * EvSimMuscle.h
 *
 *  Created on:	Jun 27, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_EVSIMMUSCLE_H_
#define SRC_EVSIMMUSCLE_H_

#include "MathStuff.h"
#include "EvSimNode.h"

typedef EvSimNode* node;

class EvSimMuscle
{
public:

	flt stdLen;
	flt wantedLen;
	node a;
	node b;

	void tick();


	EvSimMuscle();
	~EvSimMuscle();
};

#endif /* SRC_EVSIMMUSCLE_H_ */
