/*
 * ChunkManager.h
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_CHUNKMANAGER_H_
#define SRC_CHUNKMANAGER_H_

class Chunk;
#include "Chunk.h"
#include "Tickable.h"
#include "Spacevec.h"
class Frustum;
#include "Frustum.h"

class ChunkManager: public Tickable
{
	int chunkSize;//size of one chunk
	int chunksPerAxis;//width and height in chunks
	spacelen defaultH;

	Chunk ** chunks;//the currently used chunk buffer
	Chunk ** chunksBuf1;
	Chunk ** chunksBuf2;

	chunkNum lowX;//chunk coordinate of first chunk
	chunkNum lowZ;

	int renderDistanceChunks;

	spacelen gravity;

	int getIndx(spacevec abs);
	int getIndx(chunkNum cx,chunkNum cz);//from absolute chunk coordinates
	int getIndx(chunkNum cx,chunkNum cz,chunkNum newLowX,chunkNum newLowZ);//from absolute chunk coordinates
public:
	float getChunkSize();
	//float getChunkSizeInvF();
	//double getChunkSizeInvD();
	spacevec clip(spacevec pos,bool forceGround);
	bool hitsGround(spacevec startpoint,spacevec endpoint);
	spacelen getGravity();
	void tick(float time,TickServiceProvider * tsp);
	spacevec getWind(spacevec abs);
	void generateMissing(int count);
	void setMid(spacevec abs);//absolute x,z
	spacelen getHeight(spacevec abs);//absolute x,z
	void render(float lodQ,Frustum * viewFrustum, spacevec camOffset);
	flt toMeters(spacelen l);
	vec3 toMeters(spacevec v);
	spacelen fromMeters(flt l);
	spacevec fromMeters(vec3 v);
	spacevec getMiddleChunk();
	ChunkManager(int ChunkSize,int ChunksPerAxis,int RenderDistanceChunks, float gravityYdir);//render distance should be lower than half of the total chunks per axis
	~ChunkManager();
};

#endif /* SRC_CHUNKMANAGER_H_ */
