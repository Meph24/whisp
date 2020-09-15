#include "EventMapper.hpp"
#include <iostream>
#include "SFML/Window.hpp"

void EventMapping::map(EVENTMAPPING_FUNCTION_PARAMETERS)
{
	if(! condition(e, stati)) return;

	action(e, stati);
}

EventMapping::Condition operator&& (const EventMapping::Condition& c0, const EventMapping::Condition& c1)
{
	return [c0, c1](EVENTMAPPING_FUNCTION_PARAMETERS)->bool{ return c0(e, stati) && c1(e, stati); };
}
EventMapping::Condition operator|| (const EventMapping::Condition& c0, const EventMapping::Condition& c1)
{
	return [c0, c1](EVENTMAPPING_FUNCTION_PARAMETERS)->bool{ return c0(e, stati) || c1(e, stati); };
}
EventMapping::Condition operator! (const EventMapping::Condition& c0)
{
	return [c0](EVENTMAPPING_FUNCTION_PARAMETERS)->bool{ return !c0(e, stati); };
}

bool eventmapping::conditions::CALL_alwaysTrue(EVENTMAPPING_FUNCTION_PARAMETERS) { return true; }
bool eventmapping::conditions::CALL_keyPressedCall(EVENTMAPPING_FUNCTION_PARAMETERS) { return e.value > 0.0f; }
bool eventmapping::conditions::CALL_keyReleasedCall(EVENTMAPPING_FUNCTION_PARAMETERS) { return e.value <= 0.0f; }
eventmapping::conditions::EventValueEquals::EventValueEquals(float expected) 
	: expected(expected) {}
bool eventmapping::conditions::EventValueEquals::operator()(EVENTMAPPING_FUNCTION_PARAMETERS) 
{ return e.value == expected; }

eventmapping::conditions::StatusAsCondition::StatusAsCondition(bool* to_check, bool expected) 
	: to_check(to_check), expected(expected) {}
bool eventmapping::conditions::StatusAsCondition::operator()( EVENTMAPPING_FUNCTION_PARAMETERS )
{ return *to_check == expected; } 


EventMapping::Condition eventmapping::conditions::eventValueEquals(float expected) { return EventMapping::Condition(EventValueEquals(expected)); }
EventMapping::Condition eventmapping::conditions::statusAsCondition(bool* to_check, bool expected) { return EventMapping::Condition(StatusAsCondition(to_check, expected)); }

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

void EventMapper::registerMapping(int eventid, EventMapping::Action action, EventMapping::Condition condition)
{
	registerMapping(eventid, EventMapping(action, condition));	
}

void EventMapper::clearAllMappings(){ event_id_mappings.clear(); }

