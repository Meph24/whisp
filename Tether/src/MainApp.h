#pragma once

#include "SFMLHandle.h"

class MainApp
{
	/*
	//	Media Handle Instance, encapsulation of the Media Layer
	*/
	SFMLHandle sfmlHandle;

	// _test_begin
	float counter;
	// _test_end

	// executes logic int the main loop
	//_probably unnecessary due to rearrangement of loops
	void tick(int us);
public:
	MainApp();
	~MainApp();

	/*
	//	run() MUST be called explicitly
	//	starts to run the application
	*/
	void run();
	

};
