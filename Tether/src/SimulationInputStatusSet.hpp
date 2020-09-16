#ifndef CONTROLINPUTSTATUSSET_HPP
#     define CONTROLINPUTSTATUSSET_HPP

#include <array>
using std::array;

#include "glmutils.hpp"

#include <cstdint>

using SignalCounter = std::uint16_t;

//this struct shall be kept sendable, that means no dynamic allocation and pointers allowed
struct SimulationInputStatusSet
{
	//stati
	bool debug_screen_active = false;
	float restart = 0.0;
	SignalCounter inventory = 0;
	SignalCounter selection_up = 0;
	SignalCounter selection_down = 0;
	bool draw_aabbs = false;
	float benchmark = 0.0;
	bool slomo = false;
	bool pause = false;
	bool menu = false;
	float trigger = 0.0f;
	float weapon_switch = 0.0f;
	bool zoom = false;
	bool clip = false;
	bool verbose = false;
	vec3 walk = vec3(0.0f);

	//this needs to go as it is assigned from the wrong direction as of now (issue #36)
	bool selection_active = 0;

	//these methods probably need to go, as they participate in writing to this status-set
	// from the wrong side ( issue #36 )
	static void toggleCondition(bool& b);
	static void setConditionTrue(bool& b);
	static void setConditionFalse(bool& b);

	static float getStatusAndReset(float& f, float resetTo = 0.0f );//not thread safe; reset value

};

#endif /* CONTROLINPUTSTATUSSET_HPP */
