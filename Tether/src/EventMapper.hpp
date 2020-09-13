/*
 * EventMapper.hpp
 *
 *  Created on:	Sep 6, 2017
 *      Author:	HL65536, Meph24
 *     Version:	2.1
 */
#ifndef SRC_EVENTMAPPER_H_
#define SRC_EVENTMAPPER_H_

#include <unordered_map>
#include <vector>

#include "EventHandler.h"
#include "PerformanceMeter.h"

#include <functional>

using std::function;
using std::unordered_map;
using std::vector;


#include "ControlInputStatusSet.hpp"

#define EVENTMAPPING_FUNCTION_PARAMETERS EventHandler::event& e,ControlInputStatusSet& stati

using EventMappingCondition = function <bool (EVENTMAPPING_FUNCTION_PARAMETERS)>;
using EventMappingAction = function <void (EVENTMAPPING_FUNCTION_PARAMETERS)>;

struct EventMapping
{
	EventMappingAction action;
	EventMappingCondition condition;

	EventMapping(EventMappingAction action)
		: action(action), condition(conditions::alwaysTrue) {}
	EventMapping(EventMappingAction action, EventMappingCondition condition)
		: action(action), condition(condition) {}

	void map(EVENTMAPPING_FUNCTION_PARAMETERS)
	{
		if(! condition(e, stati)) return;

		action(e, stati);
	}

	struct conditions
	{
		static bool alwaysTrue(EVENTMAPPING_FUNCTION_PARAMETERS) { return true; }

		static bool keyPressed(EVENTMAPPING_FUNCTION_PARAMETERS) { return e.value > 0.0f; }
		static bool keyReleased(EVENTMAPPING_FUNCTION_PARAMETERS) { return e.value <= 0.0f; };
	
		struct StatusAsCondition
		{
			int to_check_status;
			bool expected;

			StatusAsCondition(int to_check_status, bool expected)
				: to_check_status(to_check_status), expected(expected) {}

			bool operator()(EVENTMAPPING_FUNCTION_PARAMETERS) 
			{ 
				return (expected)? 
					0 > 0.0 : 0 <= 0.0;
			}
		};
	};

	struct actions
	{
		struct Toggle
		{
			int to_toggle_status_index;
			Toggle(int to_toggle_status_index) : to_toggle_status_index(to_toggle_status_index) {}

			void toggle(float& f) { f = (f > 0.0f)? 0.0f : 1.0f; }

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { toggle( stati[to_toggle_status_index]); }
		};

		struct Combinate
		{
			int hold_group_status_index; //the status index that represents the status for the group of keys used together
			float weight;

			Combinate(int hold_group_status_index)
				: hold_group_status_index(hold_group_status_index)
				, weight(1.0f) {}
			Combinate(int hold_group_status_index, float weight) 
				: hold_group_status_index(hold_group_status_index)
				, weight(weight) {}

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { stati[hold_group_status_index] += (e.value>0)? weight : -weight; }
		};

		struct AccumulateValue
		{
			int status_index;
			float offset;

			AccumulateValue(int status_index) : status_index(status_index), offset(0.0) {}
			AccumulateValue(int status_index, float offset) : status_index(status_index), offset(offset) {}

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { stati[status_index] += e.value + offset; }
		};

		struct GetValue
		{
			int status_index;
			float offset;

			GetValue(int status_index) : status_index(status_index), offset(0.0) {}
			GetValue(int status_index, float offset) : status_index(status_index), offset(offset) {}

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { stati[status_index] = e.value + offset; }
		};
	};
};

class EventMapper
{
	ControlInputStatusSet* managed_stati;//the output/condition input
public:
	unordered_map<int, vector<EventMapping>> event_id_mappings;

	PerformanceMeter pm;

	EventMapper(ControlInputStatusSet& control_input_status_set);
	void changeManaged(ControlInputStatusSet& control_input_status_set);

	ControlInputStatusSet& stati();

	void event(EventHandler::event& e);

	void registerMapping(int eventid, EventMapping mapping);

	//be warned that this is a convenience function which, by default, the condition, with which the Mapping is constructed here is "EventMapping::conditions::alwaysTrue"
	// if some other condition would be considered a sensible default in your context (like for example toggle is usally paired with keyPressed) it is not advisable to use this function
	// with the default condition and rather specify the condition, or even pass a complete EventMapping object as stated in another method.
	void registerMapping(int eventid, EventMappingAction action, EventMappingCondition condition = EventMapping::conditions::alwaysTrue);

	void clearAllMappings();
};
#endif /* SRC_EVENTMAPPER_H_ */