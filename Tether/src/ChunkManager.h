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
#include "Pushable.h"
#include "DualPointer.h"
#include "Tickable.h"
#include "Spacevec.h"
class Frustum;
#include "Frustum.h"
#include <vector>
#include "InteractionManager.h"
#include "Drawable.h"
#include "LockFast.h"

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

class ChunkManager: public Tickable, public Drawable
{
	LockFast dataStructureLock;
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

	std::vector<chunkChange> addVec;//the entities that should be added to chunk[loc] soon
	std::vector<chunkChange> removeVec;//the entities that should be removed from chunk[loc] soon (without deleting)
	std::vector<Entity *> deleteVec;//the entities that should be removed from chunk[loc] soon (with deleting)

	int getIndxOrNeg1(spacevec abs);
	int getIndx(spacevec abs);
	int getIndx(chunkNum cx,chunkNum cz);//from absolute chunk coordinates
	int getIndx(chunkNum cx,chunkNum cz,chunkNum newLowX,chunkNum newLowZ);//from absolute chunk coordinates
	chunkSearchResult chunkSearch(Entity * e,int chunkIndx);//index -1=not found

	//search results are only valid right after acquisition
	chunkSearchResult dumbSearch(Entity * e);//index -1=not found
	chunkSearchResult smartSearch(Entity * e,spacevec pos);//index -1=not found
	chunkSearchResult trySmartSearch(Entity * e,spacevec pos,bool reportWarn);//uses dumb search and report warning (if reportWarn), if not found with smart search; index -1=not found


public:

	//magic, do not use yourself:


	void requestEntityDelete(Entity * e);//do not call this yourself, call Entiy.requestDestroy instead
	void requestEntityMove(Entity * e);//do not call yourself, managed by chunks
	spacevec activeChunk;//TODO debug only, remove after debugging
	void tick(Timestamp t,TickServiceProvider * tsp);
	void render(float lodQ,Frustum * viewFrustum, spacevec camOffset);//TODO drawable
	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp);
	void generateMissing(int count);
	spacevec getWind(spacevec abs);
	void applyEntityChunkChanges(TickServiceProvider * tsp);//only inside here entities are allowed to be added/removed from chunks, otherwise request it to be done via the request methods
	void setMid(spacevec abs,TickServiceProvider * tsp);//absolute x,z
	void clearEntities();
	ChunkManager(int ChunkSize,int ChunksPerAxis,int RenderDistanceChunks, float gravityYdir);//render distance should be lower than half of the total chunks per axis
	~ChunkManager();



	//the interface: you can safely use these:

	bool isValid(chunkNum cx,chunkNum cz);
	float getChunkSize();
	spacevec clip(spacevec pos,bool forceGround);
	bool hitsGround(spacevec startpoint,spacevec endpoint);
	spacelen getGravity();
	spacelen getHeight(spacevec abs);//absolute x,z
	flt toMeters(spacelen l);
	vec3 toMeters(spacevec v);
	spacelen fromMeters(flt l);
	spacevec fromMeters(vec3 v);
	spacevec getMiddleChunk();

	void giveInteractionManagers(Entity * e,std::vector<InteractionManager *> * managers,TickServiceProvider * tsp);


	void requestEntitySpawn(Entity * e);//spawn entity in world, call only once per entity!!


};

#endif /* SRC_CHUNKMANAGER_H_ */
