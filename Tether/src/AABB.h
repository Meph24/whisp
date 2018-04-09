/*
 * AABB.h
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_AABB_H_
#define SRC_AABB_H_

#include "MatrixLib.h"

class AABB
{
	vec3 low;
	vec3 high;
public:

	void addSpeed(vec3 displacement);//displacement=speed*time
	bool doesIntersect(AABB other);
	bool doesIntersect(AABB * other);

	void operator+=(AABB other);
	void operator+=(AABB * other);
	void operator+=(vec3 point);
	void operator+=(vec3 * point);


	AABB();
	~AABB();
};

#endif /* SRC_AABB_H_ */
