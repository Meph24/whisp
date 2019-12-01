/*
 * ITerrain.h
 *
 *  Created on:	29.11.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ITERRAIN_H_
#define SRC_ITERRAIN_H_

#include "Spacevec.h"
#include "IWorld.h"


class ITerrain
{
	IWorld * w;
	spacevec defaultGravity;
public:
	ITerrain(IWorld * world);
	virtual ~ITerrain();

	spacevec getGravity(spacevec pos);
};

#endif /* SRC_ITERRAIN_H_ */
