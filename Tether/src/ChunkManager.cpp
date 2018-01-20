/*
 * ChunkManager.cpp
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ChunkManager.h"

float ChunkManager::getHeight(flt x, flt y)
{
	int cx=getChunkCoo(x);
	int cy=getChunkCoo(y);
	if(cx<lowX) return defaultHeight;
	if(cy<lowY) return defaultHeight;
	if(cx>=(lowX+chunksPerAxis)) return defaultHeight;
	if(cy>=(lowY+chunksPerAxis)) return defaultHeight;
	if(chunks[getIndx(cx,cy)])
		return chunks[getIndx(cx,cy)]->getHeight(x,y);
	else return defaultHeight;
}

#include <iostream>
void ChunkManager::render(float lodQ)
{
	int startX=chunksPerAxis/2-renderDistanceChunks;
	int startY=chunksPerAxis/2-renderDistanceChunks;
	int stopX=startX+renderDistanceChunks*2;
	int stopY=startY+renderDistanceChunks*2;
	if(startX<0) startX=0;
	if(startY<0) startY=0;
	if(stopX>chunksPerAxis) stopX=chunksPerAxis;
	if(stopY>chunksPerAxis) stopY=chunksPerAxis;
	int midX=(startX+stopX)/2;
	int midY=(startY+stopY)/2;

	//std::cout<<"rendering from "<<startX<<" to "<<stopX<<std::endl;
	for(int runy = startY ; runy<stopY ; runy++)
	{
		for(int runx = startX ; runx<stopX ; runx++)
		{
			int indx=runy*chunksPerAxis+runx;
			if(chunks[indx])
			{
				float distX=(runx-midX);
				if (distX<0) distX=-distX;
				int distY=(runy-midY);
				if (distY<0) distY=-distY;
				int dist=distX;
				if(distY>distX) dist=distY;
				int lod=1;
				if(dist==0) dist=0.5;
				dist/=lodQ;
				if(dist>chunkSize) dist=chunkSize;
				for(int i=1;i<=dist;i++)
				{
					if((lod*2)<=i) lod*=2;
				}
				chunks[indx]->render(lod);
			}
		}
	}
	//std::cout<<"rendering end"<<std::endl;
}

ChunkManager::ChunkManager(int ChunkSize, int ChunksPerAxis,int RenderDistanceChunks):
chunkSize(ChunkSize),chunksPerAxis(ChunksPerAxis),renderDistanceChunks(RenderDistanceChunks)
{
	chunksBuf1=new Chunk * [chunksPerAxis*chunksPerAxis];
	chunksBuf2=new Chunk * [chunksPerAxis*chunksPerAxis];
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		chunksBuf1[i]=0;
		chunksBuf2[i]=0;
	}
	chunks=chunksBuf1;
	setMid(0.5f,0.5f);
}


void ChunkManager::setMid(flt x, flt y)
{
	int midChunkCoo=chunksPerAxis/2;
	int cx=getChunkCoo(x);
	int cy=getChunkCoo(y);
	int newLowX=cx-midChunkCoo;
	int newLowY=cy-midChunkCoo;

	if((newLowX!=lowX)||(newLowY!=lowY))
	{
		Chunk ** other=chunks==chunksBuf1?chunksBuf2:chunksBuf1;
		for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
		{
			other[i]=0;
		}
		int moveX=(lowX-newLowX);
		int moveY=(lowY-newLowY);
		for(int yrun=0;yrun<chunksPerAxis;yrun++)
		{
			for(int xrun=0;xrun<chunksPerAxis;xrun++)
			{
				Chunk * curChunk=chunks[yrun*chunksPerAxis+xrun];
				if(!curChunk) continue;
				int ynew=yrun+moveY;
				int xnew=xrun+moveX;
				int result=0;
				if(ynew<0) result++;
				if(ynew>=chunksPerAxis) result++;
				if(xnew<0) result++;
				if(xnew>=chunksPerAxis) result++;
				if(result)//if chunk will be outside the loaded region
				{
					delete curChunk;
				}
				else
				{
					other[ynew*chunksPerAxis+xnew]=chunks[yrun*chunksPerAxis+xrun];
				}

			}
		}
		chunks=other;
		lowX=newLowX;
		lowY=newLowY;
	}
	if(cx<lowX) return;
	if(cy<lowY) return;
	if(cx>=(lowX+chunksPerAxis)) return;
	if(cy>=(lowY+chunksPerAxis)) return;

	int myInd=getIndx(cx,cy);
	//std::cout<<"myInd = "<<myInd<<std::endl;
	if(chunks[myInd]==0)
	{
		chunks[myInd]=new Chunk(cx*chunkSize,cy*chunkSize,chunkSize);
	}
}

int ChunkManager::getChunkCoo(flt val)
{
	if(val>0) return (int)(val/chunkSize);
	return (int)(val/chunkSize)-1;
}

int ChunkManager::getIndx(int cx, int cy)
{
	cx-=lowX;
	cy-=lowY;
	return cy*chunksPerAxis+cx;
}

int ChunkManager::getIndx(int cx, int cy, int newLowX, int newLowY)
{
	cx-=newLowX;
	cy-=newLowY;
	return cy*chunksPerAxis+cx;
}

void ChunkManager::generateMissing(int count)
{
	int dir=1;
	int xrun=chunksPerAxis/2+lowX;
	int yrun=chunksPerAxis/2+lowY;
	for(int j=1;j<chunksPerAxis;j++)
	{
		dir=-dir;
		for(int i=0;i<j;i++)
		{
			xrun+=dir;
			if(chunks[getIndx(xrun,yrun)]==0)
			{
				chunks[getIndx(xrun,yrun)]=new Chunk(xrun*chunkSize,yrun*chunkSize,chunkSize);
				if(!(--count)) return;
			}
		}
		for(int i=0;i<j;i++)
		{
			yrun+=dir;
			if(chunks[getIndx(xrun,yrun)]==0)
			{
				chunks[getIndx(xrun,yrun)]=new Chunk(xrun*chunkSize,yrun*chunkSize,chunkSize);
				if(!(--count)) return;
			}
		}
	}
	dir=-dir;
	for(int i=0;i<chunksPerAxis-1;i++)
	{
		xrun+=dir;
		if(chunks[getIndx(xrun,yrun)]==0)
		{
			chunks[getIndx(xrun,yrun)]=new Chunk(xrun*chunkSize,yrun*chunkSize,chunkSize);
			if(!(--count)) return;
		}
	}
	/*for(int yrun=lowY;yrun<lowY+chunksPerAxis;yrun++)
	{
		for(int xrun=lowX;xrun<lowX+chunksPerAxis;xrun++)
		{
			if(chunks[getIndx(xrun,yrun)]==0)
			{
				chunks[getIndx(xrun,yrun)]=new Chunk(xrun*chunkSize,yrun*chunkSize,chunkSize);
				//return;
			}
		}
	}*/
}

vec3 ChunkManager::getWind(flt x, flt y)
{
	return vec3(0,0,0);//TODO
}

ChunkManager::~ChunkManager()
{
	for(int i=0;i<chunksPerAxis*chunksPerAxis;i++)
	{
		if(chunksBuf1[i]) delete chunksBuf1[i];
		if(chunksBuf2[i]) delete chunksBuf2[i];
	}
	delete chunksBuf1;
	delete chunksBuf2;
}

