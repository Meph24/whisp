/*
 * TimestampManager.h
 *
 *  Created on:	28.04.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_TIMESTAMPMANAGER_H_
#define SRC_TIMESTAMPMANAGER_H_

#include <SFML/Window.hpp>

#include "Timestamp.h"
#include "ShortNames.h"

class TimestampManager
{
	sf::Clock clock;
	i64 getCurrentTime();
	float maxMasterWallTime;
	float maxMasterGameTime;
	Timestamp lastMasterTimestamp;
	i64 lastMasterTime;
public:
	float lastMasterRate;

	float targetRate;//can be changed freely at runtime

	Timestamp masterUpdate();//update for the next tick, get timestamp for new tick

	Timestamp getSlaveTimestamp();//get time elapsed since last tick



	TimestampManager(float TargetRate,float MaxMasterWallTime,float MaxMasterGameTime);//time in ms
};

#endif /* SRC_TIMESTAMPMANAGER_H_ */
