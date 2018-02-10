/*
 * MutPart.h
 *
 *  Created on:	Aug 29, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_MUTPART_H_
#define SRC_MUTPART_H_

template<typename T>//?
class MutPart
{
public:
	long long lastTimestamp;//microseconds

	MutPart();
	~MutPart();


	float getTimeSinceTick(long long timestampNow);//since last tick
	bool wasModifiedLastAt(long long timestamp);//returns if the last modification was at the given timestamp


};

#endif /* SRC_MUTPART_H_ */
