#pragma once

#include "SFML\Window.hpp"
#include <fstream>
#include "SFML\Graphics.hpp"

#include "HardCursorHandle.h"

class MainApp
{
	sf::ContextSettings contextSettings;
	sf::Window window;
	std::ofstream MainAppInfo;
	HardCursorHandle hardcursorhandle;

	// _test_begin
	float counter;
	// _test_end

	void setClearParameters(float r, float g, float b, float a = 255, float depth = 1.0f);
	void windowClear();
	void windowClear(float r, float g, float b, float a = 255, float depth = 1.0f);

	void tick(int us);

	void preHandleEvent(sf::Event& e);
	void postHandleEvent(sf::Event& e);
public:


	MainApp();
	~MainApp();
	void run();
	friend std::ostream& operator<< (std::ostream& os, const MainApp& A);

};