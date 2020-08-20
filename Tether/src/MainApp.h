#pragma once

#include "SFMLHandle.h"
#include <memory>

using std::unique_ptr;

class MainApp
{
	/*
	//	Media Handle Instance, encapsulation of the Media Layer
	*/
	unique_ptr<SFMLHandle> sfmlHandle;

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
