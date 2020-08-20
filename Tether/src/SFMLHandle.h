#pragma once

#include "IMediaHandle.h"

#include "EventHandler.h"
#include "Buffer.h"

#include <SFML/Window.hpp>

#include "IGameMode.h"
#include <memory>

using std::unique_ptr;

struct SFMLHandle : public IMediaHandle
{
	// handler of the events
	unique_ptr<EventHandler> event_handler;


	void mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer);
	void postHandleEvent(sf::Event& e);
	void preHandleEvent(sf::Event& e);
public:


	sf::ContextSettings contextSettings;
	sf::Window window;
	SFMLHandle() = default;
	SFMLHandle(std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings);


	void operateSimulation(IGameMode* simulation);
	void disconnectSimulation();

	void createWindow(std::string name,  int reswidth, int resheight, IMediaHandle::ContextSettings& settings);
	void pollEvents();
	void setContextToMyThread();
	void display();
};

