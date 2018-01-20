/*
 * ChunkManager.h
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_CHUNKMANAGER_H_
#define SRC_CHUNKMANAGER_H_
#include "Chunk.h"
#include "MatrixLib.h"
class ChunkManager
{
	int chunkSize;//size of one chunk
	int chunksPerAxis;//width and height in chunks
	Chunk ** chunks;//the currently used chunk buffer
	Chunk ** chunksBuf1;
	Chunk ** chunksBuf2;

	int lowX;//chunk coordinate of first chunk
	int lowY;
	int renderDistanceChunks;
	int getIndx(int cx,int cy);//from absolute chunk coordinates
	int getIndx(int cx,int cy,int newLowX,int newLowY);//from absolute chunk coordinates
public:
	vec3 getWind(flt x,flt y);
	void generateMissing(int count);
	void setMid(flt x,flt y);//absolute x,ya
	float getHeight(flt x,flt y);//absolute x,y
	//void vertexH(flt x,flt offsetY,flt z);
	int getChunkCoo(flt val);//get absolute chunk coordinate from absolute coordinate
	void render(float lodQ);
	ChunkManager(int ChunkSize,int ChunksPerAxis,int RenderDistanceChunks);//render distance should be lower than half of the total chunks per axis
	~ChunkManager();
};

#endif /* SRC_CHUNKMANAGER_H_ */
