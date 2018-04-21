/*
 * DivisionPlane.h
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_DIVISIONPLANE_H_
#define SRC_DIVISIONPLANE_H_
#include "MatrixLib.h"
#include "AABB.h"
//divides space into 2 pieces
//optimized for telling on which side each of many AABBs are
class DivisionPlane
{
	int xPtr;
	int yPtr;
	int zPtr;
public:
	vec3 normal;
	flt distanceInChunks;
	DivisionPlane();
	DivisionPlane(vec3 Normal,flt Dist);
	~DivisionPlane();
	bool inside(spacelen * bb,spacevec observerPos);
	bool inside(AABB bb,spacevec observerPos);
};

#endif /* SRC_DIVISIONPLANE_H_ */
