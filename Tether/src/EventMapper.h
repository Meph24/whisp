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
//no mapping

#define MAPPER_MODE_HOLD 0
//output is the sum of all mapParams where the events are currently >0
//use case example: is button x currently pressed?; are buttons x+y or x+z currently pressed?

#define MAPPER_MODE_TOGGLE 1
//toggles the status value between 0 and 1 if event value is specified value
//use case example: toggle GUI element on/off

#define MAPPER_MODE_ADD 2
//adds the event value and mapParam to status
//use case example: how much has the user scrolled with the mouse wheel?; has the user pressed key x at any point since last check

#define MAPPER_MODE_ADD_RESETM 3
//adds the event value and mapParam to status and resets mouse position afterwards to -mapParam
//use case example: first person camera movement by mouse

#define MAPPER_MODE_REPLACE 4
//replaces status with event value+mapParam
//use case example: get current mouse position


//the modes below are not implemented yet
#define MAPPER_MODE_TIMESTAMP_GREATER 5
//TODO status = last time where event > mapParam, seconds


#define MAPPER_MODE_TIMESTAMP_LESSOREQUAL 6
//TODO status = last time where event <= mapParam




#define CONDITION_ALWAYS_TRUE 0

#include <vector>

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
	std::vector<int> ID;//input event IDs
	std::vector<mapping> m;

	PerformanceMeter pm;

	EventMapper(int statusSize=256);
	EventMapper(std::string url);
	~EventMapper();

	float getStatus(int ID);

	void toggleCondition(int ID);
	void setConditionTrue(int ID);
	void setConditionFalse(int ID);

	void setConditionORedTrue(int ID);//this works with multiple users (not thread safe) as long as they do not call these functions in an alternating pattern (true, false, true, false)
	void setConditionORedFalse(int ID);//condition will be true when at least one user wants it to be true (condition=#true calls>#false calls)

	float getTimeRef();
	void event(EventHandler::event e);

	float getStatusAndReset(int indx,float resetTo=0);//not thread safe; reset value

	void registerAction(int ID,int mode,int condition,int statusIndex,float mapParam);

	void clearAllMappings();//does not delete saved status values
};
#endif /* SRC_EVENTMAPPER_H_ */
