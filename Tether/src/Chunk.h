/*
 * Chunk.h
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_CHUNK_H_
#define SRC_CHUNK_H_

#define defaultHeight -1

#include "MatrixLib.h"
#include <GL/glew.h>
class Chunk
{
	int x;
	int y;
	int size;
	float * height;
	float avgHeight;
	GLuint bufID;

	inline float getH(int x,int y);
public:
	float getHeight(flt x,flt y);//absolute x,y
	void render(int lod);
	Chunk(int xStart,int yStart,int baseSize);//from xStart,yStart to xStart+size,yStart+size; this means chunks overlap by 1
	~Chunk();
};

#endif /* SRC_CHUNK_H_ */
