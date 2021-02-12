/*
 * AABB.h
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_AABB_H_
#define SRC_AABB_H_

#include <iostream>

#include "Drawable.h"

#include "Spacevec.h"
#include "ShortNames.h"

class AABB: public Drawable
{
private:
	vec3 convert(int bitvec);
public:
	spacevec low;
	spacevec high;


	static u64 intersectionCounter;//count number of intersections for debug/performance measurement purposes
	static u64 checkCounter;//count number of checks for debug/performance measurement purposes

	bool doesIntersect(AABB other);
	bool doesIntersect(AABB * other);
	bool contains(spacevec s);

	bool isMultichunk();

	virtual void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw, Perspective& perspective);

	void extend(AABB other);
	void extend(AABB * other);

	AABB();//uninitialized
	AABB(spacevec pos);//point-like stationary object
	AABB(spacevec pos,spacevec sizeFromMid);//stationary object
	AABB(spacevec posT1,spacevec sizeFromMid,spacevec movement);
	AABB(spacevec posT0,spacevec posT1,spacevec lowBorderOffset,spacevec highBorderOffset);
	~AABB();


};

std::ostream& operator<< (std::ostream& os, const AABB& bb);

sf::Packet& operator<<(sf::Packet& p, AABB& bb);
sf::Packet& operator>>(sf::Packet& p, AABB& bb);

#endif /* SRC_AABB_H_ */
