/*
 * MPoint.h
 *
 *  Created on:	Aug 14, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_MPOINT_H_
#define SRC_MPOINT_H_

#include "MatrixLib.h"

class MPoint
{
	//TODO add interpolation function support e.g. for circular motion
public:
	vec3 pos;
	vec3 vel;

	MPoint();
	~MPoint();

	vec3 getRenderPos(float timeSinceUpdate);

};

#endif /* SRC_MPOINT_H_ */
