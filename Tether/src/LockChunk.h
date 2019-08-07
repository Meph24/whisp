/*
 * LockChunk.h
 *
 *  Created on:	30.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_LOCKCHUNK_H_
#define SRC_LOCKCHUNK_H_
#include <vector>
#include "LockFast.h"
class Chunk;
class LockChunk
{
	LockFast myLock;
public:

	//the chunks contained by this lock chunk, subject to external change unless you own the lock
	std::vector<Chunk *> chunks;

	//do not use the lock API directly, use managers (if you can)
	//whoever changes the chunk vector or pointers in a chunk must have the corresponding lock
	bool try_lock();
	void lock();
	void unlock();



	LockChunk(int containedChunksX,int containedChunksZ);
	~LockChunk();
};
#include "Chunk.h"

#endif /* SRC_LOCKCHUNK_H_ */
