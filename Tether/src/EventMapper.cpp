#include "EventMapper.hpp"
#include <iostream>
#include "SFML/Window.hpp"

EventMapper::EventMapper(SimulationInputStatusSet& input_status)
	: managed_stati(&input_status)
	, pm(1)
{}

void EventMapper::event(EventHandler::event& e)
{
	for(EventMapping& m : event_id_mappings[e.ID])
	{
		m.map(e, *managed_stati);
	}
}

void EventMapper::registerMapping(int eventid, EventMapping mapping)
{	
	event_id_mappings[eventid].emplace_back(mapping);
}

void EventMapper::registerMapping(int eventid, EventMappingAction action, EventMappingCondition condition)
{
	registerMapping(eventid, EventMapping(action, condition));	
}

void EventMapper::clearAllMappings(){ event_id_mappings.clear(); }