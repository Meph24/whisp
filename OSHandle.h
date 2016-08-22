#pragma once

#include <SFML\Window.hpp>
#include "EventHandler.h"
#include "Buffer.h"

class OSHandle
{
	// handler of the events
	EventHandler EH;


	void mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer);
	void postHandleEvent(sf::Event& e);
	void preHandleEvent(sf::Event& e);
public:
	sf::ContextSettings contextSettings;
	sf::Window window;
	OSHandle();
	~OSHandle();

	void pollEvents();
};

