/*
 * ChunkManager.cpp
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include <iostream>

#include "Cfg.hpp"
#include "ChunkManager.h"
#include "Chunk.h"
#include "Perspective.hpp"
#include "Entity.h"
#include "Frustum.h"
#include "InteractionManager.h"
#include "LockChunk.h"
#include "WarnErrReporter.h"

ChunkManager::ChunkManager(	float terrainQuality,
							int RenderDistanceChunks, 
							int ChunkLoadRate,
							IWorld& iw)
	: ITerrain(&iw)
	, chunkLoadRate(ChunkLoadRate)
	, chunksPerAxis(RenderDistanceChunks)
	, renderDistanceChunks(RenderDistanceChunks)
	, lodQuality(terrainQuality)
{
	chunksBuf1=new Chunk * [chunksPerAxis*chunksPerAxis];
	chunksBuf2=new Chunk * [chunksPerAxis*chunksPerAxis];
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		chunksBuf1[i]=0;
		chunksBuf2[i]=0;
	}
	chunks=chunksBuf1;
	setMid({{0,0.5f},{0,0.5f},{0,0.5f}},iw);
	defaultH=iw.fromMeters(defaultHeight*1.0f);
}

spacelen ChunkManager::getHeight(spacevec abs,IWorld& iw)
{
	gridInt cx=abs.x.intpart;
	gridInt cz=abs.z.intpart;
	if(!isValid(cx,cz)) return defaultH;
	if(chunks[getIndx(cx,cz)])
		return chunks[getIndx(cx,cz)]->getHeight(abs.x.floatpart,abs.z.floatpart,iw);
	else return iw.fromMeters(defaultHeight*1.0f);
}

void ChunkManager::render(Frustum * viewFrustum, Perspective& perspective,IWorld& iw)
{
	spacevec camOffset=viewFrustum->observerPos;

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

	perspective.graphics_ressources.grass->bind();

	for(int runz = startZ ; runz<stopZ ; runz++)
	{
		for(int runx = startX ; runx<stopX ; runx++)
		{
			int indx=runz*chunksPerAxis+runx;
			if(chunks[indx]&&viewFrustum->inside(chunks[indx]->bb,iw))
			{
				//std::cout<<"rendering chunk "<<runz<<"|"<<runx<<std::endl;
				//std::cout<<"chunk bb: \n"<<chunks[indx]->bb.low<<"\n"<<chunks[indx]->bb.high<<std::endl;
				//std::cout<<"observerPos: "<<viewFrustum->observerPos<<std::endl;
				float distX=(runx-midX);
				if (distX<0) distX=-distX;
				int distZ=(runz-midZ);
				if (distZ<0) distZ=-distZ;
				int dist=distX;
				if(distZ>distX) dist=distZ;
				int lod=1;
				if(dist==0) dist=0.5;
				dist/=lodQuality;
				if(dist>iw.toMeters({1,0})) dist=iw.toMeters({1,0});
				for(int i=1;i<=dist;i++)
				{
					if((lod*2)<=i) lod*=2;
				}
				chunks[indx]->render(lod, camOffset, perspective,iw);
			}
		}
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void ChunkManager::setMid(spacevec abs,IWorld& iw)
{
	gridInt midChunkCoo=chunksPerAxis/2;
	gridInt cx=abs.x.intpart;
	gridInt cz=abs.z.intpart;
	gridInt newLowX=cx-midChunkCoo;
	gridInt newLowZ=cz-midChunkCoo;

	if((newLowX!=lowX)||(newLowZ!=lowZ))
	{
		Chunk ** other=chunks==chunksBuf1?chunksBuf2:chunksBuf1;
		for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
		{
			other[i]=0;
		}
		gridInt moveX=(lowX-newLowX);
		gridInt moveZ=(lowZ-newLowZ);
		for(int zrun=0;zrun<chunksPerAxis;zrun++)
		{
			for(int xrun=0;xrun<chunksPerAxis;xrun++)
			{
				Chunk * curChunk=chunks[zrun*chunksPerAxis+xrun];
				if(!curChunk) continue;
				gridInt znew=zrun+moveZ;
				gridInt xnew=xrun+moveX;
				int result=0;
				if(znew<0) result++;
				if(znew>=chunksPerAxis) result++;
				if(xnew<0) result++;
				if(xnew>=chunksPerAxis) result++;
				if(result)//if chunk will be outside the loaded region
				{
					delete curChunk;
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
	tryCreateChunk(cx,cz,iw);//make sure middle chunk exists so player is always in a loaded chunk
}

int ChunkManager::getIndx(gridInt cx,gridInt cz)
{
	cx-=lowX;
	cz-=lowZ;
	return cz*chunksPerAxis+cx;
}

int ChunkManager::getIndx(gridInt cx,gridInt cz,gridInt newLowX,gridInt newLowZ)
{
	cx-=newLowX;
	cz-=newLowZ;
	return cz*chunksPerAxis+cx;
}

void ChunkManager::generateMissing(int count,IWorld& iw)
{
	int dir=1;
	gridInt xrun=chunksPerAxis/2+lowX;
	gridInt zrun=chunksPerAxis/2+lowZ;
	for(int j=1;j<chunksPerAxis;j++)
	{
		dir=-dir;
		for(int i=0;i<j;i++)
		{
			xrun+=dir;
			if(tryCreateChunk(xrun,zrun,iw)) if(!(--count)) return;
		}
		for(int i=0;i<j;i++)
		{
			zrun+=dir;
			if(tryCreateChunk(xrun,zrun,iw)) if(!(--count)) return;
		}
	}
	dir=-dir;
	for(int i=0;i<chunksPerAxis-1;i++)
	{
		xrun+=dir;
		if(tryCreateChunk(xrun,zrun,iw)) if(!(--count)) return;
	}
}


int ChunkManager::getIndx(spacevec abs)
{
	return getIndx(abs.x.intpart,abs.z.intpart);
}

bool ChunkManager::tryCreateChunk(gridInt cx, gridInt cz,IWorld& iw)
{
	if(cx<lowX) return false;
	if(cz<lowZ) return false;
	if(cx>=(lowX+chunksPerAxis)) return false;
	if(cz>=(lowZ+chunksPerAxis)) return false;

	int myInd=getIndx(cx,cz);
	if(chunks[myInd]==0)
	{
		spacevec chunkBase;
		chunkBase.y.floatpart=0;
		chunkBase.y.intpart=0;
		chunkBase.x.floatpart=0;
		chunkBase.z.floatpart=0;
		chunkBase.x.intpart=cx;
		chunkBase.z.intpart=cz;
		chunks[myInd]=new Chunk(chunkBase,iw);
	}
	else return false;
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

spacevec ChunkManager::clip(spacevec pos, bool forceGround,IWorld& iw)
{
	spacevec ret=pos;
	spacelen height=getHeight(pos,iw);
	if(ret.y<height || forceGround)
	{
		ret.y=height;
	}
	return ret;
}
#define MAX_GROUNDHIT_TRIES 50
bool ChunkManager::hitsGround(spacevec startpoint, spacevec endpoint,IWorld& iw)
{
	//TODO make better
	spacevec dif=endpoint-startpoint;
	spacelen len=dif.lengthLP();
	unsigned int tries;
	if(len.intpart>MAX_GROUNDHIT_TRIES)
	{
		tries=MAX_GROUNDHIT_TRIES;
	}
	else
	{
		tries=len.intpart;
		if(tries<1) tries=1;
	}
	float triesInv=1.0f/tries;
	for(unsigned int i = 0 ; i<tries+1; i++)
	{
		spacevec testPos=startpoint+dif*triesInv;
		if(testPos.y<getHeight(testPos,iw)) return true;
	}
	return false;
}

int ChunkManager::getIndxOrNeg1(spacevec abs)
{
	gridInt x=abs.x.intpart;
	gridInt z=abs.z.intpart;
	if(isValid(x,z)) return getIndx(x,z);
	else return -1;
}


void ChunkManager::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum,IWorld& iw, Perspective& perspective)
{
	render(viewFrustum,perspective,iw);
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

bool ChunkManager::isValid(gridInt cx, gridInt cz)
{
	gridInt relLowX=cx-lowX;
	gridInt relLowZ=cz-lowZ;
	if(relLowX<0) return false;
	if(relLowZ<0) return false;
	if(relLowX>=chunksPerAxis) return false;
	if(relLowZ>=chunksPerAxis) return false;
	return true;
}


void ChunkManager::postTickTerrainCalcs(TickServiceProvider * tsp,spacevec playerPos)
{
	generateMissing(chunkLoadRate,tsp->world());//TODO load rate refactor
	setMid(playerPos,tsp->world());
}
