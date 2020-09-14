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

struct EventMapping
{
	using Condition = function <bool (EVENTMAPPING_FUNCTION_PARAMETERS)>;
	using Action = function <void (EVENTMAPPING_FUNCTION_PARAMETERS)>;
	
	Action action;
	Condition condition;

	EventMapping(Action action, Condition condition)
		: action(action), condition(condition) {}

	void map(EVENTMAPPING_FUNCTION_PARAMETERS);
};

//Note, that while these logical operators are provided for conditions
// they do not work with implicit constructors
// meaning we seem to cannot pass a function pointer in there and expect
// a std::function object imlicitly being constructed, sadly
// this means the construction of EventMapping::Conditions must be explicit in these cases
// for all predefined conditions below, there shall be a simple way to access these
// note therefore the difference between callable and function-object
EventMapping::Condition operator&& (const EventMapping::Condition& c0, const EventMapping::Condition& c1);
EventMapping::Condition operator|| (const EventMapping::Condition& c0, const EventMapping::Condition& c1);
EventMapping::Condition operator! (const EventMapping::Condition& c0);

namespace eventmapping
{
	namespace actions
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

		typedef GetValue Hold;
	} /* namespace actions */

	namespace conditions
	{
		// callables here
		bool CALL_alwaysTrue(EVENTMAPPING_FUNCTION_PARAMETERS);
		bool CALL_keyPressedCall(EVENTMAPPING_FUNCTION_PARAMETERS);
		bool CALL_keyReleasedCall(EVENTMAPPING_FUNCTION_PARAMETERS);
		struct EventValueEquals
		{
			float expected;
			EventValueEquals(float expected);
			bool operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};
		struct StatusAsCondition
		{
			int to_check_status;
			bool expected;
			StatusAsCondition(int to_check_status, bool expected);
			bool operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};

		//predefined condition-type conditions for easy combinations via operator
		const EventMapping::Condition alwaysTrue = CALL_alwaysTrue ;
		const EventMapping::Condition keyPressed = CALL_keyPressedCall;
		const EventMapping::Condition keyReleased = CALL_keyReleasedCall;
		
		EventMapping::Condition eventValueEquals(float expected);
		EventMapping::Condition statusAsCondition(int to_check_status, bool expected);
	} /* namespace conditions */
} /* namespace eventmapping */


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
	void registerMapping(int eventid, EventMapping::Action action, EventMapping::Condition condition = eventmapping::conditions::alwaysTrue);

	void clearAllMappings();
};
#endif /* SRC_EVENTMAPPER_H_ */