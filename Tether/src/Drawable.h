/*
 * Drawable.h
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_DRAWABLE_H_
#define SRC_DRAWABLE_H_

#include "SimClock.hpp"

class Frustum;
class IWorld;
class Perspective;

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;
	virtual void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw, Perspective& dsp)=0;//observerPos must already be interpolated with tickOffset
};


#endif /* SRC_DRAWABLE_H_ */
