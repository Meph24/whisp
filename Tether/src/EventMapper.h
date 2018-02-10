/*
 * EventMapper.h
 *
 *  Created on:	Sep 6, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_EVENTMAPPER_H_
#define SRC_EVENTMAPPER_H_

#define MAPPER_MODE_EMPTY -1

#define MAPPER_MODE_HOLD 0
//output is the sum of all mapParams where the events are currently >0

#define MAPPER_MODE_TOGGLE 1
//toggles if event value is specified value

#define MAPPER_MODE_ADD 2
//adds the event value and mapParam to status

#define MAPPER_MODE_ADD_RESETM 3
//adds the event value and mapParam to status and resets mouse position afterwards to -mapParam

#define MAPPER_MODE_REPLACE 4
//replaces status with event value+mapParam

#define MAPPER_MODE_TIMESTAMP_GREATER 5
//TODO status = last time where event > mapParam, seconds


#define MAPPER_MODE_TIMESTAMP_LESSOREQUAL 6
//TODO status = last time where event <= mapParam


#include "EventHandler.h"
#include "PerformanceMeter.h"

typedef struct
{
	int mode;//mapping mode, see defines
	int condition;//index of status array that needs to be 0 (if condition positive) or !=0 (if condition negative, 0 means no condition
	int statusIndex;
	float mapParam;//different meaning depending on mode
} mapping;


class EventMapper
{
public:
	//input: eventID
	//output: queryable status
	//supported tier2 options:
	//ORed inputs -> hold (different weights and directions supported)
	//inputs -> toggle on matching value
	//inputs -> add with offset
	//inputs -> replace with offset


	float * status;//the output/condition input
	int stSize;

	//input related:
	int * ID;//input event IDs
	mapping * m;
	int arrSize;

	PerformanceMeter pm;

	EventMapper(int arraySize=1024,int statusSize=256);
	EventMapper(std::string url);
	~EventMapper();

	float getStatus(int ID);
	float getTimeRef();
	void event(EventHandler::event e);
	//void registerMapping();
	float getStatusAndReset(int indx,float resetTo=0);
	void registerAction(int ID,int mode,int condition,int statusIndex,float mapParam);
};
#endif /* SRC_EVENTMAPPER_H_ */
