/*
 * chunkCooHash.h
 *
 *  Created on:	20.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_CHUNKCOOHASH_H_
#define SRC_CHUNKCOOHASH_H_


#include "xxhash.h"
#include "Spacevec.h"

struct chunkCooHash
{
	std::size_t operator() (const chunkCoo &c) const
	{
		XXH64_hash_t hash = XXH64(&c, sizeof(chunkCoo), 42);
		return hash;
	}
};



#endif /* SRC_CHUNKCOOHASH_H_ */
