#pragma once

#include "SFML\Window.hpp"
#include <fstream>
#include "SFML\Graphics.hpp"

#include "OSHandle.h"
#include "Graphics.h"

#include "HardCursorHandle.h"

class MainApp
{
	OSHandle oshandle;
	Graphics graphics;
	std::ofstream MainAppInfo;

	// _test_begin
	float counter;
	// _test_end

	
	void tick(int us);

	
public:


	MainApp();
	~MainApp();
	void run();
	

};