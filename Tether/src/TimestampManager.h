/*
 * TimestampManager.h
 *
 *  Created on:	28.04.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_TIMESTAMPMANAGER_H_
#define SRC_TIMESTAMPMANAGER_H_

#include "Timestamp.h"
#include "ShortNames.h"

class TimestampManager
{
public:
	Timestamp lastUpdated;
	i64 lastKnownTime;
	float curTimescale;
	float wantedTimescale;

	Timestamp getNextTimestamp();//TODO

	TimestampManager();
	~TimestampManager();
};

#endif /* SRC_TIMESTAMPMANAGER_H_ */
