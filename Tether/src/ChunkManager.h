/*
 * ChunkManager.h
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_CHUNKMANAGER_H_
#define SRC_CHUNKMANAGER_H_

#include "IWorld.h"
#include "ITerrain.h"

#include "Spacevec.h"

class Cfg;
class Chunk;
class Frustum;
class InteractionManager;

#include <memory>
#include <vector>

#include"glmutils.hpp"
using glm::vec3;


class ChunkManager:public ITerrain
{
	int chunkLoadRate;
	int chunksPerAxis;//width and height in chunks
	spacelen defaultH;

	Chunk ** chunks;//the currently used chunk buffer
	Chunk ** chunksBuf1;
	Chunk ** chunksBuf2;

	gridInt lowX;//chunk coordinate of first chunk
	gridInt lowZ;

	int renderDistanceChunks;


	int getIndxOrNeg1(spacevec abs);
	int getIndx(spacevec abs);
	int getIndx(gridInt cx,gridInt cz);//from absolute chunk coordinates
	int getIndx(gridInt cx,gridInt cz,gridInt newLowX,gridInt newLowZ);//from absolute chunk coordinates

	bool isValid(gridInt cx,gridInt cz);

	bool tryCreateChunk(gridInt cx,gridInt cz,IWorld& iw);
	bool insideLimits(int x,int z,int maxX,int maxZ);

	spacelen getHeight(spacevec abs,IWorld& iw);//absolute x,z

	float lodQuality = 0.5;

public:

	void render(Frustum * viewFrustum, Perspective& perspective,IWorld& iw);
	virtual void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum, IWorld& iw, Perspective& perspective);
	void generateMissing(int count,IWorld& iw);
	void setMid(spacevec abs,IWorld& iw);//absolute x,z

	//chunksPerLockchunk must be power of 2, if its not, it will be assigned one
	ChunkManager(	float terrainQuality,
					int RenderDistanceChunks,
					int ChunkLoadRate,
					IWorld& iw
				);//render distance should be lower than half of the total chunks per axis
	~ChunkManager();

	///the non-critical interface: you can always safely use these

	//the partially critical interface: only call from main tick thread
	//read-only:
	spacevec getMiddleChunk();//TODO remove

	//from ITerrain interface:
	spacevec clip(spacevec pos,bool forceGround,IWorld& iw);
	bool hitsGround(spacevec startpoint,spacevec endpoint,IWorld& iw);
	void postTickTerrainCalcs(TickServiceProvider * tsp,spacevec playerPos);//centers chunks on player
};

#endif /* SRC_CHUNKMANAGER_H_ */
