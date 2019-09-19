/*
 * ChunkManager.cpp
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ChunkManager.h"
#include "Chunk.h"
#include "Entity.h"
#include "InteractionManager.h"
#include "LockChunk.h"
#include "Frustum.h"


ChunkManager::ChunkManager(int ChunkSize,int ChunksPerAxis,int RenderDistanceChunks, int chunksPerLockchunk, float gravityYdir):
chunksPerAxis(ChunksPerAxis),renderDistanceChunks(RenderDistanceChunks)
{
	gridSize=ChunkSize;
	lockChunkSizeX=1;
	lockChunkSizeZ=1;
	while(chunksPerLockchunk>3)
	{
		lockChunkSizeX*=2;
		lockChunkSizeZ*=2;
		chunksPerLockchunk/=4;
	}
	if(chunksPerLockchunk>1)
	{
		lockChunkSizeX*=2;
	}
	lockChunkStartX=0;
	lockChunkStartZ=0;
	lockChunksPerAxisX=(chunksPerAxis+lockChunkSizeX*2-2)/lockChunkSizeX;
	lockChunksPerAxisZ=(chunksPerAxis+lockChunkSizeZ*2-2)/lockChunkSizeZ;
	chunksBuf1=new Chunk * [chunksPerAxis*chunksPerAxis];
	chunksBuf2=new Chunk * [chunksPerAxis*chunksPerAxis];
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		chunksBuf1[i]=0;
		chunksBuf2[i]=0;
	}
	chunks=chunksBuf1;
	setMid({{0,0.5f},{0,0.5f},{0,0.5f}},0);
	gravity=fromMeters(gravityYdir);
	defaultH=fromMeters(defaultHeight*1.0f);
}

spacelen ChunkManager::getHeight(spacevec abs)
{
	chunkNum cx=abs.x.intpart;
	chunkNum cz=abs.z.intpart;
	if(!isValid(cx,cz)) return defaultH;
	if(chunks[getIndx(cx,cz)])
		return chunks[getIndx(cx,cz)]->getHeight(abs.x.floatpart,abs.z.floatpart);
	else return fromMeters(defaultHeight*1.0f);
}

#include <iostream>
void ChunkManager::render(float lodQ,Frustum * viewFrustum, spacevec camOffset)
{


	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	int startX=chunksPerAxis/2-renderDistanceChunks;
	int startZ=chunksPerAxis/2-renderDistanceChunks;
	int stopX=startX+renderDistanceChunks*2;
	int stopZ=startZ+renderDistanceChunks*2;
	if(startX<0) startX=0;
	if(startZ<0) startZ=0;
	if(stopX>chunksPerAxis) stopX=chunksPerAxis;
	if(stopZ>chunksPerAxis) stopZ=chunksPerAxis;
	int midX=(startX+stopX)/2;
	int midZ=(startZ+stopZ)/2;

	for(int runz = startZ ; runz<stopZ ; runz++)
	{
		for(int runx = startX ; runx<stopX ; runx++)
		{
			int indx=runz*chunksPerAxis+runx;
			if(chunks[indx]&&viewFrustum->inside(chunks[indx]->bb,this))
			{
				float distX=(runx-midX);
				if (distX<0) distX=-distX;
				int distZ=(runz-midZ);
				if (distZ<0) distZ=-distZ;
				int dist=distX;
				if(distZ>distX) dist=distZ;
				int lod=1;
				if(dist==0) dist=0.5;
				dist/=lodQ;
				if(dist>gridSize) dist=gridSize;
				for(int i=1;i<=dist;i++)
				{
					if((lod*2)<=i) lod*=2;
				}
				chunks[indx]->render(lod, camOffset);
			}
		}
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

#include "WarnErrReporter.h"
void ChunkManager::setMid(spacevec abs,TickServiceProvider * tsp)
{
	chunkNum midChunkCoo=chunksPerAxis/2;
	chunkNum cx=abs.x.intpart;
	chunkNum cz=abs.z.intpart;
	chunkNum newLowX=cx-midChunkCoo;
	chunkNum newLowZ=cz-midChunkCoo;

	if((newLowX!=lowX)||(newLowZ!=lowZ))
	{
		Chunk ** other=chunks==chunksBuf1?chunksBuf2:chunksBuf1;
		for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
		{
			other[i]=0;
		}
		chunkNum moveX=(lowX-newLowX);
		chunkNum moveZ=(lowZ-newLowZ);
		for(int zrun=0;zrun<chunksPerAxis;zrun++)
		{
			for(int xrun=0;xrun<chunksPerAxis;xrun++)
			{
				Chunk * curChunk=chunks[zrun*chunksPerAxis+xrun];
				if(!curChunk) continue;
				chunkNum znew=zrun+moveZ;
				chunkNum xnew=xrun+moveX;
				int result=0;
				if(znew<0) result++;
				if(znew>=chunksPerAxis) result++;
				if(xnew<0) result++;
				if(xnew>=chunksPerAxis) result++;
				if(result)//if chunk will be outside the loaded region
				{
					int size=curChunk->managedEntities.size();
					for(int i=0;i<size;i++)
					{
						curChunk->managedEntities[i]->refCounter--;
						if(curChunk->managedEntities[i]->refCounter)
						{
							WarnErrReporter::doubleErr("entity found in multiple chunks while one of them is destroyed, ignoring it");
							continue;
						}
						curChunk->managedEntities[i]->onLeaveWorld(tsp);
					}
					delete curChunk;//TODO managed entities
				}
				else
				{
					other[znew*chunksPerAxis+xnew]=chunks[zrun*chunksPerAxis+xrun];
				}

			}
		}
		chunks=other;
		lowX=newLowX;
		lowZ=newLowZ;
	}
	//correctLockChunks();
	tryCreateChunk(cx,cz);//make sure middle chunk exists so player is always in a loaded chunk
}

int ChunkManager::getIndx(chunkNum cx,chunkNum cz)
{
	cx-=lowX;
	cz-=lowZ;
	return cz*chunksPerAxis+cx;
}

int ChunkManager::getIndx(chunkNum cx,chunkNum cz,chunkNum newLowX,chunkNum newLowZ)
{
	cx-=newLowX;
	cz-=newLowZ;
	return cz*chunksPerAxis+cx;
}

void ChunkManager::generateMissing(int count)
{
	int dir=1;
	chunkNum xrun=chunksPerAxis/2+lowX;
	chunkNum zrun=chunksPerAxis/2+lowZ;
	for(int j=1;j<chunksPerAxis;j++)
	{
		dir=-dir;
		for(int i=0;i<j;i++)
		{
			xrun+=dir;
			if(tryCreateChunk(xrun,zrun)) if(!(--count)) return;
		}
		for(int i=0;i<j;i++)
		{
			zrun+=dir;
			if(tryCreateChunk(xrun,zrun)) if(!(--count)) return;
		}
	}
	dir=-dir;
	for(int i=0;i<chunksPerAxis-1;i++)
	{
		xrun+=dir;
		if(tryCreateChunk(xrun,zrun)) if(!(--count)) return;
	}
}


void ChunkManager::tick(Timestamp t, TickServiceProvider* tsp)
{
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		if(chunks[i]) chunks[i]->tick(t,tsp);
	}
}

int ChunkManager::getIndx(spacevec abs)
{
	return getIndx(abs.x.intpart,abs.z.intpart);
}

spacelen ChunkManager::getGravity()
{
	return gravity;
}

void ChunkManager::correctLockChunks()
{
	chunkNum highX=lowX+chunksPerAxis;
	chunkNum highZ=lowZ+chunksPerAxis;
	chunkNum lockChunkStopX=lockChunkStartX+lockChunksPerAxisX*lockChunkSizeX;
	chunkNum lockChunkStopZ=lockChunkStartZ+lockChunksPerAxisZ*lockChunkSizeZ;
	if ((lowX-lockChunkStartX>=0)&&
		(lowZ-lockChunkStartZ>=0)&&
		(lockChunkStopX-highX>=0)&&
		(lockChunkStopZ-highZ>=0))
	{
		return;//no need to change anything
	}

	int size=lockChunks.size();
	std::vector<std::shared_ptr<LockChunk>> tempBuf(size);//TODO correct way to preallocate?
	for(int i=0;i<size;i++)
	{
		tempBuf.push_back(lockChunks[i]);
	}

	chunkNum newX=lockChunkStartX;
	chunkNum newZ=lockChunkStartZ;
	if(lockChunkStartX-lowX>0)
	{
		chunkNum correctionSize=(lockChunkStartX-lowX+lockChunkSizeX-1)/lockChunkSizeX;
		newX-=correctionSize*lockChunkSizeX;
	}
	else if(highX-lockChunkStopX>0)
	{
		chunkNum correctionSize=(lockChunkStartX-lowX+lockChunkSizeX-1)/lockChunkSizeX;
		newX+=correctionSize*lockChunkSizeX;
	}
	if(lockChunkStartZ-lowZ>0)
	{
		chunkNum correctionSize=(lockChunkStartZ-lowZ+lockChunkSizeZ-1)/lockChunkSizeZ;
		newZ-=correctionSize*lockChunkSizeZ;
	}
	else if(highZ-lockChunkStopZ>0)
	{
		chunkNum correctionSize=(lockChunkStartZ-lowZ+lockChunkSizeZ-1)/lockChunkSizeZ;
		newZ+=correctionSize*lockChunkSizeZ;
	}

	for(int zrun=0;zrun<lockChunksPerAxisZ;zrun++)
	{
		for(int xrun=0;xrun<lockChunksPerAxisX;xrun++)
		{
			int newInd=zrun*lockChunksPerAxisX+xrun;
			chunkNum realX=newX+xrun*lockChunkSizeX;
			chunkNum realZ=newZ+zrun*lockChunkSizeZ;
			int oldX=realX/lockChunkSizeX-lockChunkStartX;
			int oldZ=realZ/lockChunkSizeZ-lockChunkStartZ;
			int oldInd=oldZ*lockChunksPerAxisX+oldX;

			if(insideLimits(oldX,oldZ,lockChunksPerAxisX,lockChunksPerAxisZ))
			{
				tempBuf[newInd]=lockChunks[oldInd];//TODO check if correct
				lockChunks[oldInd]=std::shared_ptr<LockChunk>(0);//TODO check if correct
			}
			else
			{
				tempBuf[newInd]=std::shared_ptr<LockChunk>(new LockChunk(lockChunkSizeX,lockChunkSizeZ));//TODO more?
			}
		}
	}
	/*
	int size=lockChunksPerAxisX*lockChunksPerAxisZ;
	for(int i=0;i<size;i++)
	{
		if(lockChunks[i])//non-null in old structure
		{
			lockChunks[i]->lock();

			//make sure every contained chunk is null

			lockChunks[i]->unlock();
			//free the own shared ptr//TODO make sure shared pointer free properly deletes lockchunk
		}
	}
*/
	//TODO

	lockChunks=tempBuf;//TODO replace by non-dummy
}

bool ChunkManager::tryCreateChunk(chunkNum cx, chunkNum cz)
{
	if(cx<lowX) return false;
	if(cz<lowZ) return false;
	if(cx>=(lowX+chunksPerAxis)) return false;
	if(cz>=(lowZ+chunksPerAxis)) return false;

	int myInd=getIndx(cx,cz);
	if(chunks[myInd]==0)
	{
		spacevec chunkBase;
		chunkBase.y=fromMeters(0);
		chunkBase.x.floatpart=0;
		chunkBase.z.floatpart=0;
		chunkBase.x.intpart=cx;
		chunkBase.z.intpart=cz;
		chunks[myInd]=new Chunk(chunkBase,gridSize,this);
	}
	else return false;
	//TODO lockchunks
	return true;
}

bool ChunkManager::insideLimits(int x, int z, int maxX, int maxZ)
{
	if(x<0) return false;
	if(z<0) return false;
	if(x>=maxX) return false;
	if(z>=maxZ) return false;
	return true;
}

spacevec ChunkManager::clip(spacevec pos, bool forceGround)
{
	spacevec ret=pos;
	spacelen height=getHeight(pos);
	if(ret.y<height || forceGround)
	{
		ret.y=height;
	}
	return ret;
}
#define MAX_GROUNDHIT_TRIES 50
bool ChunkManager::hitsGround(spacevec startpoint, spacevec endpoint)
{
	//TODO make better
	spacevec dif=endpoint-startpoint;
	float len=dif.fLength(getChunkSize());
	//std::cout<<len<<std::endl;


	int tries;
	if(len>MAX_GROUNDHIT_TRIES)
	{
		tries=MAX_GROUNDHIT_TRIES;
	}
	else
	{
		tries=int(len);
		if(tries<1) tries=1;
	}
	float triesInv=1.0f/tries;
	for(int i = 0 ; i<tries+1; i++)
	{
		spacevec testPos=startpoint+dif*triesInv;
		if(testPos.y<getHeight(testPos)) return true;
	}
	return false;
}

#include "myAssert.h"
void ChunkManager::giveInteractionManagers(Entity* e,std::vector<InteractionManager*> * managers,TickServiceProvider * tsp)
{
	chunkNum minx=e->bb.low.x.intpart;
	chunkNum maxx=e->bb.high.x.intpart;
	chunkNum minz=e->bb.low.z.intpart;
	chunkNum maxz=e->bb.high.z.intpart;

	if((maxx-minx)>=10 )
	{
		std::cout<<"fail: wrong bb; x:"<<minx<<"-"<<maxx<<"z:"<<minz<<"-"<<maxz<<std::endl;
	}
	assert((maxx-minx)<10);
	assert((maxz-minz)<10);
	int tickID=tsp->tickID;
	for(chunkNum xrun=minx;xrun<=maxx;xrun++)
	{
		for(chunkNum zrun=minz;zrun<=maxz;zrun++)
		{
			if(isValid(xrun,zrun))
			{
				int indx=getIndx(xrun,zrun);
				if(indx<0 || indx>chunksPerAxis*chunksPerAxis)
					std::cout<<"fail: wrong index: "<<indx<<std::endl;
				else
				if(chunks[indx])
				{
					if(chunks[indx]->lastTickID!=tickID)
					{
						chunks[indx]->interMan->resetAll();
						chunks[indx]->lastTickID=tickID;
					}
					managers->push_back(chunks[indx]->interMan);
				}
			}
		}
	}
}
#include "WarnErrReporter.h"
void ChunkManager::applyEntityChunkChanges(TickServiceProvider * tsp)
{
	int size=addVec.size();
	for(int i=0;i<size;i++)
	{
		chunkChange c=addVec[i];
		int indx=getIndxOrNeg1(c.loc);
		if(indx==-1)
		{
			if(c.e->refCounter==0)//freshly spawned outside
			{
				c.e->onLeaveWorld(tsp);//handle by calling the Entity and letting it decide what to do (save to disk/simple deletion)
			}
			continue;
		}
		if(!chunks[indx])
		{
			//TODO generate chunks smarter
			while(!chunks[indx]) generateMissing(10);
		}
		chunks[indx]->managedEntities.push_back(c.e);
		c.e->residentPos=c.loc;
		c.e->refCounter++;
	}
	addVec.clear();
	size=removeVec.size();
	for(int i=0;i<size;i++)
	{
		chunkChange c=removeVec[i];
		chunkSearchResult res=trySmartSearch(c.e,c.loc,true);
		if(res.chunkIndex==-1)
		{
			WarnErrReporter::notFoundErr("entity to remove could not be found, doing nothing...");
			if(c.e->refCounter>1) WarnErrReporter::doubleErr("entity now has 2 chunks");
		}
		else
		{
			Chunk * myChunk=chunks[res.chunkIndex];
			if(c.e!=myChunk->managedEntities[res.vectorIndex])
			{
				WarnErrReporter::notFoundErr("entity was not where search results indicate, doing nothing...");
				if(c.e->refCounter>1) WarnErrReporter::doubleErr("entity now has 2 chunks");
				continue;
			}
			int size=myChunk->managedEntities.size();
			if(size-1==res.vectorIndex)
			{
				myChunk->managedEntities.pop_back();
			}
			else
			{
				myChunk->managedEntities[res.vectorIndex]=myChunk->managedEntities[size-1];
				myChunk->managedEntities.pop_back();
			}
			c.e->refCounter--;
			if(c.e->refCounter==0)//moved outside
			{
				c.e->onLeaveWorld(tsp);//handle by calling the Entity and letting it decide what to do (save to disk/simple deletion)
			}
		}
	}
	removeVec.clear();
	size=deleteVec.size();
	for(int i=0;i<size;i++)
	{
		Entity * e=deleteVec[i];
		if(e->refCounter<=0)
		{
			delete e;
			continue;
		}
		else if(e->refCounter>1)
		{
			WarnErrReporter::doubleErr("entity is in more than one chunk, deleting it anyway...");
			//TODO cleanup of remaining refs
		}
		chunkSearchResult res=trySmartSearch(e,e->residentPos,true);
		if(res.chunkIndex==-1)
		{
			WarnErrReporter::notFoundErr("entity was not found despite refCounter>0, deleting it anyway...");
			delete e;
		}
		else
		{
			Chunk * myChunk=chunks[res.chunkIndex];
			if(e!=myChunk->managedEntities[res.vectorIndex])
			{
				WarnErrReporter::notFoundErr("entity was not where search results indicate, deleting it anyway...");
				delete e;
				continue;
			}
			int size=myChunk->managedEntities.size();
			if(size-1==res.vectorIndex)
			{
				myChunk->managedEntities.pop_back();
				delete e;
			}
			else
			{
				myChunk->managedEntities[res.vectorIndex]=myChunk->managedEntities[size-1];
				myChunk->managedEntities.pop_back();
				delete e;
			}
		}
	}
	deleteVec.clear();
}

void ChunkManager::requestEntitySpawn(Entity* e)
{
	addVec.push_back({e,e->pos});//capture the position in this exact moment as the insert location
}

void ChunkManager::requestEntityDelete(Entity* e)
{
	deleteVec.push_back(e);
}

void ChunkManager::requestEntityMove(Entity* e)
{
	addVec.push_back({e,e->pos});//capture the position in this exact moment as the new chunk location
	removeVec.push_back({e,e->residentPos});
}

int ChunkManager::getIndxOrNeg1(spacevec abs)
{
	chunkNum x=abs.x.intpart;
	chunkNum z=abs.z.intpart;
	if(isValid(x,z)) return getIndx(x,z);
	else return -1;
}

chunkSearchResult ChunkManager::dumbSearch(Entity* e)
{
	chunkSearchResult result;
	result.chunkIndex=-1;
	result.vectorIndex=-1;
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		result=chunkSearch(e,i);
		if(result.chunkIndex!=-1)
		{
			return result;
		}
	}
	return result;
}

chunkSearchResult ChunkManager::smartSearch(Entity* e, spacevec pos)
{
	chunkSearchResult ret;
	ret.chunkIndex=getIndxOrNeg1(pos);
	if(ret.chunkIndex==-1)
	{
		ret.vectorIndex=-1;
		return ret;
	}
	return chunkSearch(e,ret.chunkIndex);
}

void ChunkManager::draw(Timestamp t, Frustum* viewFrustum, ChunkManager* cm,DrawServiceProvider* dsp)
{
	int startX=chunksPerAxis/2-renderDistanceChunks;
	int startZ=chunksPerAxis/2-renderDistanceChunks;
	int stopX=startX+renderDistanceChunks*2;
	int stopZ=startZ+renderDistanceChunks*2;
	if(startX<0) startX=0;
	if(startZ<0) startZ=0;
	if(stopX>chunksPerAxis) stopX=chunksPerAxis;
	if(stopZ>chunksPerAxis) stopZ=chunksPerAxis;

	for(int runz = startZ ; runz<stopZ ; runz++)
	{
		for(int runx = startX ; runx<stopX ; runx++)
		{
			int indx=runz*chunksPerAxis+runx;
			if(chunks[indx])
			{
				chunks[indx]->draw(t,viewFrustum,cm,dsp);
			}
		}
	}
}

void ChunkManager::clearEntities()
{
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		if(chunks[i]) chunks[i]->clearEntities();
	}
}

chunkSearchResult ChunkManager::trySmartSearch(Entity* e, spacevec pos,bool reportWarn)
{
	chunkSearchResult result=smartSearch(e,pos);
	if(result.chunkIndex!=-1) return result;
	result=dumbSearch(e);
	if(reportWarn)
	{
		if(result.chunkIndex==-1) WarnErrReporter::notFoundErr("Entity not found");
		else WarnErrReporter::notFoundWhereBelongsErr("Entity not found at stated location");
	}
	return result;
}

chunkSearchResult ChunkManager::chunkSearch(Entity* e, int chunkIndx)
{
	chunkSearchResult ret;
	if(!chunks[chunkIndx])
	{
		ret.chunkIndex=-1;
		ret.vectorIndex=-1;
		return ret;
	}
	std::vector<Entity *> * myVec=&(chunks[chunkIndx]->managedEntities);
	int size=myVec->size();
	for(int i=0;i<size;i++)
	{
		if((*myVec)[i]==e)
		{
			ret.chunkIndex=chunkIndx;
			ret.vectorIndex=i;
			return ret;
		}
	}
	ret.chunkIndex=-1;
	ret.vectorIndex=-1;
	return ret;
}

ChunkManager::~ChunkManager()
{
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		if(chunksBuf1[i]) delete chunksBuf1[i];
		if(chunksBuf2[i]) delete chunksBuf2[i];//TODO correct?
	}
	delete chunksBuf1;
	delete chunksBuf2;
}

spacevec ChunkManager::getMiddleChunk()
{
	spacevec ret;
	ret.x.floatpart=0;
	ret.y.floatpart=0;
	ret.z.floatpart=0;
	ret.x.intpart=lowX+chunksPerAxis/2;
	ret.y.intpart=0;
	ret.z.intpart=lowZ+chunksPerAxis/2;
	return ret;
}

float ChunkManager::getChunkSize()
{
	return gridSize;
}

bool ChunkManager::isValid(chunkNum cx, chunkNum cz)
{
	chunkNum relLowX=cx-lowX;
	chunkNum relLowZ=cz-lowZ;
	if(relLowX<0) return false;
	if(relLowZ<0) return false;
	if(relLowX>=chunksPerAxis) return false;
	if(relLowZ>=chunksPerAxis) return false;
	return true;
}