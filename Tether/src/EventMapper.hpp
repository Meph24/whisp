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

#define EVENTMAPPING_FUNCTION_PARAMETERS EventHandler::event& e, SimulationInputStatusSet& stati

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
			bool* to_check;
			bool expected;

			StatusAsCondition(bool *to_check, bool expected)
				: to_check(to_check), expected(expected) {}

			bool operator()(EVENTMAPPING_FUNCTION_PARAMETERS) 
			{ 
				return (*to_check == expected);
			}
		};
	};

	struct actions
	{
		struct Toggle
		{
			bool* to_toggle;
			Toggle(bool* to_toggle)
				: to_toggle(to_toggle) {}

			void toggle(bool& b) { b = (b)? false : true; }

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { toggle(*to_toggle); }
		};

		struct Combinate
		{
			float* hold_group_accumulation_variable; //the status index that represents the status for the group of keys used together
			float weight;

			Combinate(float* hold_group_accumulation_variable)
				: hold_group_accumulation_variable(hold_group_accumulation_variable)
				, weight(1.0f) {}
			Combinate(float* hold_group_accumulation_variable, float weight) 
				: hold_group_accumulation_variable(hold_group_accumulation_variable)
				, weight(weight) {}

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { *hold_group_accumulation_variable += (e.value>0)? weight : -weight; }
		};

		struct AccumulateValue
		{
			float* accumulation_variable;
			float offset;

			AccumulateValue(float* accumulation_variable) :  accumulation_variable(accumulation_variable), offset(0.0) {}
			AccumulateValue(float* accumulation_variable, float offset) : accumulation_variable(accumulation_variable), offset(offset) {}

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { *accumulation_variable += e.value + offset; }
		};

		struct SendSignal
		{
			SimulationInputStatusSet::SignalChannel* signal_channel;
			SendSignal(SimulationInputStatusSet::SignalChannel* signal_channel) : signal_channel(signal_channel) {}
			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { ++(*signal_channel); };
		};

		struct Replace
		{
			float* replace_target;
			float offset;

			Replace(float* replace_target) :  replace_target(replace_target), offset(0.0) {}
			Replace(float* replace_target, float offset) : replace_target(replace_target), offset(offset) {}

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { *replace_target = (float)e.value + offset; }
		};
	};
};

class EventMapper
{
	SimulationInputStatusSet* managed_stati;//the output/condition input
public:
	unordered_map<int, vector<EventMapping>> event_id_mappings;

	PerformanceMeter pm;

	EventMapper(SimulationInputStatusSet& input_status_);
	void changeManaged(SimulationInputStatusSet& input_status);

	SimulationInputStatusSet& stati();

	void event(EventHandler::event& e);

	void registerMapping(int eventid, EventMapping mapping);

	//be warned that this is a convenience function which, by default, the condition, with which the Mapping is constructed here is "EventMapping::conditions::alwaysTrue"
	// if some other condition would be considered a sensible default in your context (like for example toggle is usally paired with keyPressed) it is not advisable to use this function
	// with the default condition and rather specify the condition, or even pass a complete EventMapping object as stated in another method.
	void registerMapping(int eventid, EventMappingAction action, EventMappingCondition condition = EventMapping::conditions::alwaysTrue);

	void clearAllMappings();
};
#endif /* SRC_EVENTMAPPER_H_ */