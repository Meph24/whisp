/*
 * Compatibility.h
 *
 *  Created on:	12.12.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_COMPATIBILITY_H_
#define SRC_COMPATIBILITY_H_

class Compatibility
{
	unsigned int ID;//same ID=same compatibility behaviour
	//ID>>24 gives the size, smaller sizes fit in larger things but not the other way round
	//ID<<8 / 256 gives the type
public:
	Compatibility(int type,int size);
	Compatibility();
	~Compatibility();
	bool fitsInsideMe(Compatibility other);
	bool doIFitInside(Compatibility other);
};

#endif /* SRC_COMPATIBILITY_H_ */
