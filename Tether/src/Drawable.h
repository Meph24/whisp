/*
 * Drawable.h
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_DRAWABLE_H_
#define SRC_DRAWABLE_H_

#include "Timestamp.h"

class DrawServiceProvider;
class IWorld;
class Frustum;

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;
	virtual void draw(Timestamp t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp)=0;//observerPos must already be interpolated with tickOffset
};


#endif /* SRC_DRAWABLE_H_ */
