#pragma once

#include "Operator.hpp"
#include <memory>
#include "WallClock.hpp"
#include "SFMLClock.hpp"

using std::unique_ptr;

class MainApp
{
	SFMLClock base_clock;
	WallClock clock;

	/*
	//	Media Handle Instance, encapsulation of the Media Layer
	*/
	unique_ptr<Operator> op;

	unique_ptr<IGameMode> sim;

	// _test_begin
	float counter;
	// _test_end

	// executes logic int the main loop
	//_probably unnecessary due to rearrangement of loops
	void tick(int us);
public:
	MainApp();

	/*
	//	run() MUST be called explicitly
	//	starts to run the application
	*/
	void run();
	

};
