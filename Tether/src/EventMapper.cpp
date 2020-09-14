#include "EventMapper.hpp"
#include <iostream>
#include "SFML/Window.hpp"

EventMapper::EventMapper(ControlInputStatusSet& control_input_status_set)
	: managed_stati(&control_input_status_set)
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

void EventMapper::registerMapping(int eventid, EventMapping::Action action, EventMapping::Condition condition)
{
	registerMapping(eventid, EventMapping(action, condition));	
}

void EventMapper::clearAllMappings(){ event_id_mappings.clear(); }