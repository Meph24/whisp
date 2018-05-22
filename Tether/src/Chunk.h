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
#include "InteractionGroup1.h"
#include "DualPointer.h"
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
	int lastTickID=0;

//	InteractionGroup1<Pushable> * group;
	InteractionManager * interMan;


	std::vector<Entity *> coreEntities;//must be ticked by this chunk//TODO

	inline float getH(int x,int y);//inside chunk grid
public:
	AABB bb;
	void tick(Timestamp t,TickServiceProvider * tsp);
	spacelen getHeight(flt x,flt z);//coordinates inside chunk
	void render(int lod,spacevec camOffset);

	void registerCollisionCheck(DualPointer<Pushable> e, float time,TickServiceProvider* tsp);

	Chunk(spacevec basePos,int baseSize,ChunkManager * cm);//from xStart,yStart to xStart+size,yStart+size; this means chunks overlap by 1
	~Chunk();
};

#endif /* SRC_CHUNK_H_ */
