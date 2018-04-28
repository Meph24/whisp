/*
 * Drawable.h
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_DRAWABLE_H_
#define SRC_DRAWABLE_H_

class DrawServiceProvider;
#include "DrawServiceProvider.h"
class ChunkManager;
#include "ChunkManager.h"
#include "Spacevec.h"
#include "Frustum.h"
class Drawable
{
public:
	Drawable();
	virtual ~Drawable();
	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp)=0;//observerPos must already be interpolated with tickOffset
};

#endif /* SRC_DRAWABLE_H_ */
