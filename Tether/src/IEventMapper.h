#ifndef SRC_IEVENTMAPPER_H_
#define SRC_IEVENTMAPPER_H_

#include "EventHandler.h"

class IEventMapper
{
public:
	IEventMapper() = default;
	virtual ~IEventMapper() = default;
	virtual void event(EventHandler::event e)=0;
};

#endif /* SRC_IEVENTMAPPER_H_ */
