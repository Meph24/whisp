/*
 * Frustum.h
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_FRUSTUM_H_
#define SRC_FRUSTUM_H_

#include "DivisionPlane.h"
#include "AABB.h"

class IWorld;
class ITexture;


#define FRUSTUM_PLANE_COUNT 5
//must be 5 or 6
//6 uses near plane


class Frustum
{
public:
	DivisionPlane planes[FRUSTUM_PLANE_COUNT];
	spacevec observerPos;//normal and dist of DivisionPlane are relative to that
	//recommended order:
	//1st: lower plane if player looks down, upper plane otherwise
	//2nd/3rd: left+right planes
	//4th: upper plane if player looks down, lower plane otherwise
	//5th: distance plane
	//6th (not used with FRUSTUM_PLANE_COUNT==5): near plane: gain from it would be 0 most of the time, so not included here

	void debugDraw(ITexture * tex,IWorld * w);

	bool inside(spacelen * bb);
	bool inside(AABB bb);
	Frustum();
	~Frustum();
};

#endif /* SRC_FRUSTUM_H_ */
