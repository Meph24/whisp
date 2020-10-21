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

class Operator;

#include "SimulationInputStatusSet.hpp"
#include "InputDeviceConfigurator.hpp"
#include "Window.hpp"

#define EVENTMAPPING_FUNCTION_PARAMETERS EventHandler::event& e, SimulationInputStatusSet& stati

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
		enum KeyTrigger
		{ 
			on_key_press = 1,
			on_key_release = 2, 
			on_key_press_or_release = 3, 
			NUM_KEYTRIGGER
		};

		struct Toggle
		{
			bool* to_toggle;
			KeyTrigger trigger;
			Toggle(bool* to_toggle, KeyTrigger trigger = KeyTrigger::on_key_press);
			void toggle(bool& b);
			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};

		struct Combinate
		{
			float* hold_group_accumulation_variable; //the status index that represents the status for the group of keys used together
			float weight;

			Combinate(float* hold_group_accumulation_variable);
			Combinate(float* hold_group_accumulation_variable, float weight);
			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};

		template<typename AccumulationType>
		struct AccumulateValue
		{
			AccumulationType* accumulation_variable;

			AccumulateValue(AccumulationType* accumulation_variable) : accumulation_variable(accumulation_variable) {};

			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS) { *accumulation_variable += (AccumulationType)e.value; };
		};

		struct Replace
		{
			float* replace_target;
			float offset;
			Replace(float* replace_target);
			Replace(float* replace_target, float offset);
			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};

		typedef Replace Hold;

		struct SendSignal
		{
			SignalCounter* signal_counter;
			SendSignal(SignalCounter* signal_counter);
			void sendSignal(SignalCounter* signal_counter);
			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};

		struct MouseDiffInput
		{
			InputDeviceConfigurator& conf;
			Window& window;
			float* diff_accumulation;
			enum Axis{ X = 0, Y = 1, NUM_AXES };
			Axis window_axis;
			MouseDiffInput(Window& window, InputDeviceConfigurator& conf, float* diff_accumulation, Axis window_axis);
			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};
		struct ToggleMouseMode
		{
			InputDeviceConfigurator& conf;
			KeyTrigger trigger;
			ToggleMouseMode(InputDeviceConfigurator& conf, KeyTrigger trigger = KeyTrigger::on_key_press);
			void operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};

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
			bool* to_check;
			bool expected;
			StatusAsCondition(bool *to_check, bool expected);
			bool operator()(EVENTMAPPING_FUNCTION_PARAMETERS);
		};

		//predefined condition-type conditions for easy combinations via operator
		const EventMapping::Condition alwaysTrue = CALL_alwaysTrue ;
		const EventMapping::Condition keyPressed = CALL_keyPressedCall;
		const EventMapping::Condition keyReleased = CALL_keyReleasedCall;
		
		EventMapping::Condition eventValueEquals(float expected);
		EventMapping::Condition statusAsCondition(bool* to_check, bool expected);
	} /* namespace conditions */
} /* namespace eventmapping */


class EventMapper
{
public:
	SimulationInputStatusSet* managed_stati;//the output/condition input

	unordered_map<int, vector<EventMapping>> event_id_mappings;

	PerformanceMeter pm;

	EventMapper(SimulationInputStatusSet& input_status_);
	void changeManaged(SimulationInputStatusSet& input_status);

	void event(EventHandler::event& e);

	void registerMapping(int eventid, EventMapping mapping);

	//be warned that this is a convenience function which, by default, the condition, with which the Mapping is constructed here is "EventMapping::conditions::alwaysTrue"
	// if some other condition would be considered a sensible default in your context (like for example toggle is usally paired with keyPressed) it is not advisable to use this function
	// with the default condition and rather specify the condition, or even pass a complete EventMapping object as stated in another method.
	void registerMapping(int eventid, EventMapping::Action action, EventMapping::Condition condition = eventmapping::conditions::alwaysTrue);

	void clearMappings( int eventid );

	void clearAllMappings();
};
#endif /* SRC_EVENTMAPPER_H_ */