#pragma once

#include <SFML\Window.hpp>
#include "EventHandler.h"


class OSHandle
{

	EventHandler EH;


	void postHandleEvent(sf::Event& e);
	void preHandleEvent(sf::Event& e);
public:
	sf::ContextSettings contextSettings;
	sf::Window window;
	OSHandle();
	~OSHandle();

	void pollEvents();
};

