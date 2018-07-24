/*
 * Chunk.h
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_CHUNK_H_
#define SRC_CHUNK_H_

#define defaultHeight -1
#include "Pushable.h"
//#include "InteractionGroup1.h"
#include "DualPointer.h"
#include "Spacevec.h"
#include "Tickable.h"
#include "MatrixLib.h"
#include <GL/glew.h>
class ChunkManager;
#include "ChunkManager.h"
#include <vector>
#include "AABB.h"
class InteractionManager;

class Chunk: public Tickable //TODO Drawable?
{
	spacevec base;
	spacelen defaultH;
	int size;//in meters=gridUnits
	float * height;//TODO spacelen
	float avgHeight;
	//GLuint bufID;
	ChunkManager * parent;


	inline float getH(int x,int y);//inside chunk grid
public:
	std::vector<Entity *> managedEntities;//must be ticked by this chunk, usually entities that are located in this chunk (with a few exceptions)

	int lastTickID=0;
	InteractionManager * interMan;
	AABB bb;
	void tick(Timestamp t,TickServiceProvider * tsp);
	spacelen getHeight(flt x,flt z);//coordinates inside chunk
	void render(int lod,spacevec camOffset);

	Chunk(spacevec basePos,int baseSize,ChunkManager * cm);//from xStart,yStart to xStart+size,yStart+size; this means chunks overlap by 1
	~Chunk();
};
#include "InteractionManager.h"

#endif /* SRC_CHUNK_H_ */
