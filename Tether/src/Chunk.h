/*
 * Chunk.h
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_CHUNK_H_
#define SRC_CHUNK_H_

#include "Tickable.h"
#include "Drawable.h"

#include "AABB.h"

class ChunkManager;
class InteractionManager;

#include <GL/glew.h>
#include <vector>


#define defaultHeight -1

//#include "InteractionGroup1.h"

class Chunk: public Tickable, public Drawable
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
	void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp);
	spacelen getHeight(float x,float z);//coordinates inside chunk
	void render(int lod,spacevec camOffset);//TODO replace
	virtual void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);
	void clearEntities();

	Chunk(spacevec basePos,int baseSize,ChunkManager * cm);//from xStart,yStart to xStart+size,yStart+size; this means chunks overlap by 1
	~Chunk();
};

#endif /* SRC_CHUNK_H_ */
