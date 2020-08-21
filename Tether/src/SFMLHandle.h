#pragma once

#include "IMediaHandle.h"

#include "EventHandler.h"
#include "Buffer.h"
#include "WallClock.hpp"

#include <SFML/Window.hpp>

struct SFMLHandle : public IMediaHandle
{
	// handler of the events
	EventHandler EH;


	void mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer);
	void postHandleEvent(sf::Event& e);
	void preHandleEvent(sf::Event& e);
public:


	WallClock* wallclock;

	sf::ContextSettings contextSettings;
	sf::Window window;
	SFMLHandle();
	SFMLHandle(std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings);
	~SFMLHandle();


	void createWindow(std::string name,  int reswidth, int resheight, IMediaHandle::ContextSettings& settings);
	void pollEvents();
	void setContextToMyThread();
	void display();
};

