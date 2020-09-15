#ifndef CONTROLINPUTSTATUSSET_HPP
#     define CONTROLINPUTSTATUSSET_HPP

#include <array>
using std::array;

#define CONTROLINPUTSTATUSSET_MAXCONTROLS 256

struct ControlInputStatusSet : public array<float, CONTROLINPUTSTATUSSET_MAXCONTROLS>
{
	ControlInputStatusSet();

	using array::operator[];

	float status(size_type ID);

	void toggleCondition(size_type ID);
	void setConditionTrue(size_type ID);
	void setConditionFalse(size_type ID);

	void setConditionORedTrue(size_type ID);//this works with multiple users (not thread safe) as long as they do not call these functions in an alternating pattern (true, false, true, false)
	void setConditionORedFalse(size_type ID);//condition will be true when at least one user wants it to be true (condition=#true calls>#false calls)

	float getStatusAndReset(int indx,float resetTo=0);//not thread safe; reset value
};

#include "glmutils.hpp"


struct SimulationInputStatusSet
{
	using SignalChannel = unsigned short;

	//stati
	bool debug_screen_active;
	SignalChannel restart;
	SignalChannel inventory;
	float selection_up;
	float selection_down;
	bool draw_aabbs;
	float go_up;
	float go_down;
	SignalChannel benchmark;
	bool slomo;
	bool pause;
	bool menu;
	float trigger;
	float weapon_switch;
	bool zoom;
	vec3 walk;

	//this needs to go as it is assigned from the wrong direction as of now (issue #36)
	bool selection_active;

	SimulationInputStatusSet()
	: debug_screen_active(false)
	, restart(0)
	, inventory(0)
	, selection_up(0.0f)
	, selection_down(0.0f)
	, draw_aabbs(false)
	, go_up(0.0f)
	, go_down(0.0f)
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
