/*
 * IEventMapper.h
 *
 *  Created on:	Sep 7, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_IEVENTMAPPER_H_
#define SRC_IEVENTMAPPER_H_

#include "EventHandler.h"

class IEventMapper
{
public:
	IEventMapper();
	virtual ~IEventMapper();
	virtual void event(EventHandler::event e)=0;
};

#endif /* SRC_IEVENTMAPPER_H_ */
