/*
 * Timestamp.h
 *
 *  Created on:	28.04.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_TIMESTAMP_H_
#define SRC_TIMESTAMP_H_


struct Timestamp
{
	int time;//microseconds; only differences are meaningful (max 30min)

	float operator-(Timestamp other);//returns time difference in 0

	Timestamp(int microseconds);
	Timestamp();
};

#endif /* SRC_TIMESTAMP_H_ */
