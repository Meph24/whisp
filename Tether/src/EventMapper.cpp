#include "EventMapper.hpp"
#include <iostream>
#include "SFML/Window.hpp"

#include "User.hpp"

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


eventmapping::actions::Toggle::Toggle(bool* to_toggle, eventmapping::actions::KeyTrigger trigger) : to_toggle(to_toggle), trigger(trigger){}
void eventmapping::actions::Toggle::toggle(bool& b) { b = (b)? false : true; }
void eventmapping::actions::Toggle::operator()(EVENTMAPPING_FUNCTION_PARAMETERS) 
{
	if(	
			((trigger & eventmapping::actions::KeyTrigger::on_key_press) && e.value > 0.0)
		||	((trigger & eventmapping::actions::KeyTrigger::on_key_release) && e.value <= 0)
	) 
	toggle(*to_toggle); 
}


eventmapping::actions::Combinate::Combinate(float* hold_group_accumulation_variable)
	: hold_group_accumulation_variable(hold_group_accumulation_variable)
	, weight(1.0f) {}
eventmapping::actions::Combinate::Combinate(float* hold_group_accumulation_variable, float weight) 
	: hold_group_accumulation_variable(hold_group_accumulation_variable)
	, weight(weight) {}
void eventmapping::actions::Combinate::operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { *hold_group_accumulation_variable += (e.value>0)? weight : -weight; }

eventmapping::actions::Replace::Replace(float* replace_target) :  replace_target(replace_target), offset(0.0) {}
eventmapping::actions::Replace::Replace(float* replace_target, float offset) : replace_target(replace_target), offset(offset) {}
void eventmapping::actions::Replace::operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { *replace_target = (float)e.value + offset; }

eventmapping::actions::SendSignal::SendSignal(SignalCounter* signal_counter) : signal_counter(signal_counter) {}
void eventmapping::actions::SendSignal::sendSignal(SignalCounter* signal_counter) { (*signal_counter)++; }
void eventmapping::actions::SendSignal::operator()(EVENTMAPPING_FUNCTION_PARAMETERS)
{
	sendSignal(signal_counter); 
}


eventmapping::actions::MouseDiffInput::MouseDiffInput(Window& window, InputDeviceConfigurator& conf, float* diff_accumulation, Axis window_axis) 
				: conf(conf)
				, window(window)
				, diff_accumulation(diff_accumulation)
				, window_axis(window_axis)
{}

void eventmapping::actions::MouseDiffInput::operator()(EVENTMAPPING_FUNCTION_PARAMETERS)
{
	int pos = e.value;
	int size = (window_axis) ? window.height() : window.width();
	float sensitivity = (window_axis) ? conf.turnSensitivity().y : conf.turnSensitivity().x;
	//size from mid gets accumulated
	float addition = ((float)(pos - size/2)) * sensitivity;
	*diff_accumulation += addition;
	//note reset of mouse cursor to middle of window is done in post event processing (currently in User::postHandleEvent())
}

eventmapping::actions::ToggleMouseMode::ToggleMouseMode(InputDeviceConfigurator& conf, eventmapping::actions::KeyTrigger trigger) : conf(conf), trigger(trigger){}
void eventmapping::actions::ToggleMouseMode::operator()(EVENTMAPPING_FUNCTION_PARAMETERS) 
{
	if(	
			((trigger & eventmapping::actions::KeyTrigger::on_key_press) && e.value > 0.0)
		||	((trigger & eventmapping::actions::KeyTrigger::on_key_release) && e.value <= 0)
	) 
	{
		conf.setMouseMode( conf.mouseMode() == InputDeviceConfigurator::MouseMode::pointer ? InputDeviceConfigurator::MouseMode::diff : InputDeviceConfigurator::MouseMode::pointer );
	}
	
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
	, pm(8s,0s)
{}

void EventMapper::mapEvent(InputEvent& e)
{
	for(EventMapping& m : event_id_mappings[e.id])
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

void EventMapper::clearMappings( int eventid )
{
	event_id_mappings[eventid].clear();
}

void EventMapper::clearAllMappings(){ event_id_mappings.clear(); }