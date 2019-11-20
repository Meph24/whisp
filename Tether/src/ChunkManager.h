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

#include "Spacevec.h"
#include "LockFast.h"

class LockChunk;
class Chunk;
class Frustum;
class InteractionManager;

#include <memory>
#include <vector>

#include"glmutils.hpp"
using glm::vec3;

struct chunkChange
{
	Entity * e;
	spacevec loc;
};

struct chunkSearchResult
{
	int chunkIndex;
	int vectorIndex;
};

class ChunkManager: public IWorld
{
	LockFast dataStructureLock;
	int chunksPerAxis;//width and height in chunks
	spacelen defaultH;

	Chunk ** chunks;//the currently used chunk buffer
	Chunk ** chunksBuf1;
	Chunk ** chunksBuf2;

	gridInt lowX;//chunk coordinate of first chunk
	gridInt lowZ;

	int renderDistanceChunks;

	int lockChunkSizeX;
	int lockChunkSizeZ;
	gridInt lockChunkStartX;
	gridInt lockChunkStartZ;
	int lockChunksPerAxisX;
	int lockChunksPerAxisZ;


	spacelen gravity;

	std::vector<chunkChange> addVec;//the entities that should be added to chunk[loc] soon
	std::vector<chunkChange> removeVec;//the entities that should be removed from chunk[loc] soon (without deleting)

	std::vector<std::shared_ptr<LockChunk>> lockChunks;

	int getIndxOrNeg1(spacevec abs);
	int getIndx(spacevec abs);
	int getIndx(gridInt cx,gridInt cz);//from absolute chunk coordinates
	int getIndx(gridInt cx,gridInt cz,gridInt newLowX,gridInt newLowZ);//from absolute chunk coordinates
	chunkSearchResult chunkSearch(Entity * e,int chunkIndx);//index -1=not found

	//search results are only valid right after acquisition
	chunkSearchResult dumbSearch(Entity * e);//index -1=not found
	chunkSearchResult smartSearch(Entity * e,spacevec pos);//index -1=not found
	chunkSearchResult trySmartSearch(Entity * e,spacevec pos,bool reportWarn);//uses dumb search and report warning (if reportWarn), if not found with smart search; index -1=not found

	bool isValid(gridInt cx,gridInt cz);

	void correctLockChunks();//does nothing if already correct

	bool tryCreateChunk(gridInt cx,gridInt cz);
	bool insideLimits(int x,int z,int maxX,int maxZ);

	//TODO void deleteChunk()

public:

	//magic, do not use yourself:

	void requestEntityMove(Entity * e);//do not call yourself, managed by chunks
	spacevec activeChunk;//TODO debug only, remove after debugging
	void tick(Timestamp t,TickServiceProvider * tsp);
	void render(float lodQ,Frustum * viewFrustum, spacevec camOffset);//TODO drawable
	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp);
	void generateMissing(int count);
	void applyEntityChunkChanges(TickServiceProvider * tsp);//only inside here entities are allowed to be added/removed from chunks, otherwise request it to be done via the request methods
	void setMid(spacevec abs,TickServiceProvider * tsp);//absolute x,z
	void clearEntities();

	//chunksPerLockchunk must be power of 2, if its not, it will be assigned one
	ChunkManager(int ChunkSize,int ChunksPerAxis,int RenderDistanceChunks,int chunksPerLockchunk, float gravityYdir);//render distance should be lower than half of the total chunks per axis
	~ChunkManager();

	///the non-critical interface: you can always safely use these

	bool drawAABBs=false;
	spacelen getGravity();//currently a constant

	//the partially critical interface: only call from main tick thread
	//writing:
	void giveInteractionManagers(Entity * e,std::vector<InteractionManager *> * managers,TickServiceProvider * tsp);
	//read-only:
	spacelen getHeight(spacevec abs);//absolute x,z
	spacevec getMiddleChunk();
	bool hitsGround(spacevec startpoint,spacevec endpoint);
	spacevec clip(spacevec pos,bool forceGround);

	//IWorld interface:
	void requestEntitySpawn(Entity * e);//spawn entity in world, call only once per entity!!! Can fail if not within loaded chunks.



};

#endif /* SRC_CHUNKMANAGER_H_ */
