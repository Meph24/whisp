/*
 * AABB.h
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_AABB_H_
#define SRC_AABB_H_

#include "Spacevec.h"
#include "ShortNames.h"

class AABB
{
public:
	spacevec low;
	spacevec high;


	static u64 intersectionCounter;//count number of intersections for debug/performance measurement purposes
	static u64 checkCounter;//count number of checks for debug/performance measurement purposes

	bool doesIntersect(AABB other);
	bool doesIntersect(AABB * other);

	bool isMultichunk();

	AABB();//uninitialized
	AABB(spacevec pos);//point-like stationary object
	AABB(spacevec pos,spacevec sizeFromMid);//stationary object
	AABB(spacevec pos,spacevec sizeFromMid,spacevec movement);
	~AABB();
};

#endif /* SRC_AABB_H_ */
