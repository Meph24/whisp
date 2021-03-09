/*
 * Chunk.h
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_CHUNK_H_
#define SRC_CHUNK_H_

#include "AABB.h"

#include <GL/glew.h>
#include <vector>

#define defaultHeight -1

//#include "InteractionGroup1.h"

class Chunk
{
	spacevec base;
	spacelen defaultH;
	int size;//in meters=gridUnits
	float * height;//TODO spacelen
	float avgHeight;
	//GLuint bufID;

	inline float getH(int x,int y);//inside chunk grid
public:
	AABB bb;
	spacelen getHeight(float x,float z,IWorld& iw);//coordinates inside chunk
	void render(int lod,spacevec camOffset, Perspective& perspective,IWorld& iw);

	Chunk(spacevec basePos,IWorld& iw);//from xStart,yStart to xStart+size,yStart+size; this means chunks overlap by 1
	~Chunk();
};

#endif /* SRC_CHUNK_H_ */
