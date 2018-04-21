/*
 * ChunkManager.cpp
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ChunkManager.h"

ChunkManager::ChunkManager(int ChunkSize,int ChunksPerAxis,int RenderDistanceChunks, float gravityYdir):
chunkSize(ChunkSize),chunksPerAxis(ChunksPerAxis),renderDistanceChunks(RenderDistanceChunks)
{
	//fChunkSizeInv=1.0f/chunkSize;
	//dChunkSizeInv=1.0/chunkSize;
	chunksBuf1=new Chunk * [chunksPerAxis*chunksPerAxis];
	chunksBuf2=new Chunk * [chunksPerAxis*chunksPerAxis];
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		chunksBuf1[i]=0;
		chunksBuf2[i]=0;
	}
	chunks=chunksBuf1;
	setMid({{0,0.5f},{0,0.5f},{0,0.5f}});
	gravity=fromMeters(gravityYdir);
	defaultH=fromMeters(defaultHeight*1.0f);
}

spacelen ChunkManager::getHeight(spacevec abs)
{
	chunkNum cx=abs.x.intpart;
	chunkNum cz=abs.z.intpart;
	chunkNum relLowX=cx-lowX;
	chunkNum relLowZ=cz-lowZ;
	if(relLowX<0) return defaultH;
	if(relLowZ<0) return defaultH;
	if(relLowX>=chunksPerAxis) return defaultH;
	if(relLowZ>=chunksPerAxis) return defaultH;
	if(chunks[getIndx(cx,cz)])
		return chunks[getIndx(cx,cz)]->getHeight(abs.x.floatpart,abs.z.floatpart);
	else return fromMeters(defaultHeight*1.0f);
}

#include <iostream>
void ChunkManager::render(float lodQ,Frustum * viewFrustum, spacevec camOffset)
{
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

	//std::cout<<"rendering from "<<startX<<" to "<<stopX<<std::endl;
	for(int runz = startZ ; runz<stopZ ; runz++)
	{
		for(int runx = startX ; runx<stopX ; runx++)
		{
			int indx=runz*chunksPerAxis+runx;
			if(chunks[indx]&&viewFrustum->inside(chunks[indx]->bb))
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
				if(dist>chunkSize) dist=chunkSize;
				for(int i=1;i<=dist;i++)
				{
					if((lod*2)<=i) lod*=2;
				}
				chunks[indx]->render(lod, camOffset);
			}
		}
	}
	//std::cout<<"rendering end"<<std::endl;
}


void ChunkManager::setMid(spacevec abs)
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
	if(cx<lowX) return;
	if(cz<lowZ) return;
	if(cx>=(lowX+chunksPerAxis)) return;
	if(cz>=(lowZ+chunksPerAxis)) return;

	int myInd=getIndx(cx,cz);
	//std::cout<<"myInd = "<<myInd<<std::endl;
	if(chunks[myInd]==0)
	{
		spacevec chunkBase;
		chunkBase.y=fromMeters(0);
		chunkBase.x.floatpart=0;
		chunkBase.z.floatpart=0;
		chunkBase.x.intpart=cx;
		chunkBase.z.intpart=cz;
		chunks[myInd]=new Chunk(chunkBase,chunkSize,this);
	}
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
			if(chunks[getIndx(xrun,zrun)]==0)
			{
				spacevec chunkBase;
				chunkBase.y=fromMeters(0);
				chunkBase.x.floatpart=0;
				chunkBase.z.floatpart=0;
				chunkBase.x.intpart=xrun;
				chunkBase.z.intpart=zrun;
				chunks[getIndx(xrun,zrun)]=new Chunk(chunkBase,chunkSize,this);
				if(!(--count)) return;
			}
		}
		for(int i=0;i<j;i++)
		{
			zrun+=dir;
			if(chunks[getIndx(xrun,zrun)]==0)
			{
				spacevec chunkBase;
				chunkBase.y=fromMeters(0);
				chunkBase.x.floatpart=0;
				chunkBase.z.floatpart=0;
				chunkBase.x.intpart=xrun;
				chunkBase.z.intpart=zrun;
				chunks[getIndx(xrun,zrun)]=new Chunk(chunkBase,chunkSize,this);
				if(!(--count)) return;
			}
		}
	}
	dir=-dir;
	for(int i=0;i<chunksPerAxis-1;i++)
	{
		xrun+=dir;
		if(chunks[getIndx(xrun,zrun)]==0)
		{
			spacevec chunkBase;
			chunkBase.y=fromMeters(0);
			chunkBase.x.floatpart=0;
			chunkBase.z.floatpart=0;
			chunkBase.x.intpart=xrun;
			chunkBase.z.intpart=zrun;
			chunks[getIndx(xrun,zrun)]=new Chunk(chunkBase,chunkSize,this);
			if(!(--count)) return;
		}
	}
}

spacevec ChunkManager::getWind(spacevec abs)
{
	return {{0,0},{0,0},{0,0}};//TODO
}

void ChunkManager::tick(float time, TickServiceProvider* tsp)
{
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		if(chunks[i]) chunks[i]->tick(time,tsp);
	}
}

int ChunkManager::getIndx(spacevec abs)
{
	return getIndx(abs.x.intpart,abs.z.intpart);
}

flt ChunkManager::toMeters(spacelen l)
{
	flt ret=l.floatpart;
	ret+=l.intpart;
	ret*=chunkSize;
	return ret;
}

vec3 ChunkManager::toMeters(spacevec v)
{
	vec3 ret=vec3(v.x.floatpart,v.y.floatpart,v.z.floatpart);
	ret+=vec3(v.x.intpart,v.y.intpart,v.z.intpart);
	ret=ret*chunkSize;
	return ret;
}

spacelen ChunkManager::fromMeters(flt l)
{
	l/=chunkSize;
	spacelen ret;
	ret.floatpart=l;
	ret.intpart=0;
	ret.correct();
	return ret;
}

spacevec ChunkManager::fromMeters(vec3 v)
{
	spacevec ret;
	ret.x=fromMeters(v.x);
	ret.y=fromMeters(v.y);
	ret.z=fromMeters(v.z);
	return ret;
}

spacelen ChunkManager::getGravity()
{
	return gravity;
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
	return chunkSize;
}
