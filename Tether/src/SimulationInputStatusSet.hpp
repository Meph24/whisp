#ifndef CONTROLINPUTSTATUSSET_HPP
#     define CONTROLINPUTSTATUSSET_HPP

#include <array>
using std::array;

#include "glmutils.hpp"

using SignalCounter = unsigned int;

//this struct shall be kept sendable, that means no dynamic allocation and pointers allowed
struct SimulationInputStatusSet
{

	//stati
	bool debug_screen_active;
	float restart;
	SignalCounter inventory;
	SignalCounter selection_up;
	SignalCounter selection_down;
	bool draw_aabbs;
	float benchmark;
	bool slomo;
	bool pause;
	bool menu;
	float trigger;
	float weapon_switch;
	bool zoom;
	bool clip;
	vec3 walk;

	//this needs to go as it is assigned from the wrong direction as of now (issue #36)
	bool selection_active;

	SimulationInputStatusSet()
	: debug_screen_active(false)
	, restart(0)
	, inventory(0.0f)
	, selection_up(0.0f)
	, selection_down(0.0f)
	, draw_aabbs(false)
	, benchmark(0)
	, slomo(false)
	, pause(false)
	, menu(false)
	, trigger(0.0f)
	, weapon_switch(0.0f)
	, zoom(false)
	, walk(vec3(0.0f))
	{}


	//these methods probably need to go, as they participate in writing to this status-set
	// from the wrong side ( issue #36 )
	static void toggleCondition(bool& b);
	static void setConditionTrue(bool& b);
	static void setConditionFalse(bool& b);

	static float getStatusAndReset(float& f, float resetTo = 0.0f );//not thread safe; reset value

};

#endif /* CONTROLINPUTSTATUSSET_HPP */
