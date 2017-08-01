/*
 * EvSimNode.h
 *
 *  Created on:	Jun 27, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */


#ifndef SRC_EVSIMNODE_H_
#define SRC_EVSIMNODE_H_

#include "MatrixLib.h"

class EvSimNode
{
public:

	vec3 pos;
	vec3 vel;
	flt mass;
	flt airFriction;
	flt groundFriction;//ground friction is calculated instead of air friction, not in addition to it. This value holds therefore the combined effects of friction with ground and air (purpose=to save computation time)



	EvSimNode();
	~EvSimNode();
};

#endif /* SRC_EVSIMNODE_H_ */
