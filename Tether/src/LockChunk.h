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
class LockChunk
{
public:
	std::vector<int> chunkInd;//the chunks contained by this lock chunk


	//whoever changes pointers in a chunk must have the corresponding lock
	void lockBlocking();
	void freeLock();

	LockChunk();
	~LockChunk();
};

#endif /* SRC_LOCKCHUNK_H_ */
