/*
 * IRandom.h
 *
 *  Created on:	Nov 27, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_IRANDOM_H_
#define SRC_IRANDOM_H_
#include "ShortNames.h"

//class-based RNGs that are NOT singletons
class IRandom
{
private:
	u64 seed;
public:
	IRandom(u64 Seed);
	IRandom();//generate seed randomly (quality not guaranteed, may contain current time)
	~IRandom();
};

#endif /* SRC_IRANDOM_H_ */
