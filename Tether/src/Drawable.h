/*
 * Drawable.h
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_DRAWABLE_H_
#define SRC_DRAWABLE_H_

class Drawable
{
public:
	Drawable();
	virtual ~Drawable();
	virtual void draw(float tickOffset,spacevec observerPos,ChunkManager * cm,DrawServiceProvider * dsp)=0;//observerPos must also be interpolated with tickOffset
};

#endif /* SRC_DRAWABLE_H_ */
