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
class ChunkManager;
class Frustum;

class Drawable
{
public:
	Drawable();
	virtual ~Drawable();
	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp)=0;//observerPos must already be interpolated with tickOffset
};


//ugly hack: fuck c++'s include ordering bullshit
#ifndef SRC_ENTITY_H_
#include "DrawServiceProvider.h"
#include "Frustum.h"
#include "ChunkManager.h"
#endif /* SRC_ENTITY_H_ */
//do not include yet if coming from Entity.h


#endif /* SRC_DRAWABLE_H_ */
