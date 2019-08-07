/*
 * LockChunk.cpp
 *
 *  Created on:	30.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "LockChunk.h"
#include "WarnErrReporter.h"

LockChunk::LockChunk(int containedChunksX,int containedChunksZ)
{
	for(int i=0;i<containedChunksX*containedChunksZ;i++)
	{
		chunks.push_back(0);
	}
}

bool LockChunk::try_lock()
{
	return myLock.try_lock();
}

void LockChunk::lock()
{
	myLock.lock();
}

void LockChunk::unlock()
{
	myLock.unlock();
}

LockChunk::~LockChunk()
{
	int size=chunks.size();
	for(int i=0;i<size;i++)
	{
		if(chunks[i]) WarnErrReporter::notDeletedErr("LockChunk deleted before it was empty");
	}
}

