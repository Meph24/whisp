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
#include "Spacevec.h"
#include "Tickable.h"
#include "MatrixLib.h"
#include <GL/glew.h>
class ChunkManager;
#include "ChunkManager.h"
#include <vector>
#include "AABB.h"

class Chunk: public Tickable //TODO Drawable?
{
	spacevec base;
	spacelen defaultH;
	int size;//in meters=gridUnits
	float * height;//TODO spacelen
	float avgHeight;
	//GLuint bufID;
	ChunkManager * parent;
	//std::vector<Entity *> coreEntities;//must be ticked by this chunk
	//std::vector<Entity *> sideEntities;//must not be ticked by this chunk

	inline float getH(int x,int y);//inside chunk grid
public:
	AABB bb;
	void tick(float time,TickServiceProvider * tsp);
	spacelen getHeight(flt x,flt z);//coordinates inside chunk
	void render(int lod,spacevec camOffset);
	Chunk(spacevec basePos,int baseSize,ChunkManager * cm);//from xStart,yStart to xStart+size,yStart+size; this means chunks overlap by 1
	~Chunk();
};

#endif /* SRC_CHUNK_H_ */
