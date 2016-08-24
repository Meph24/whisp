#pragma once

#include "SFML\Window.hpp"
#include <fstream>
#include "SFML\Graphics.hpp"

#include "SFMLHandle.h"
#include "Graphics.h"

#include "HardCursorHandle.h"

class MainApp
{
	SFMLHandle SFMLHandle;
	Graphics graphics;

	// _test_begin
	float counter;
	// _test_end

	
	void tick(int us);

	
public:


	MainApp();
	~MainApp();
	void run();
	

};