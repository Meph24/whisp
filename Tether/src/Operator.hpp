#ifndef OPERATOR_HPP
#     define OPERATOR_HPP

#include "IMediaHandle.h"

#include "EventHandler.h"
#include "EventMapper.hpp"
#include "Buffer.h"
#include "WallClock.hpp"

#include <SFML/Window.hpp>

#include "IGameMode.h"
#include <memory>

using std::unique_ptr;

struct Operator : public IMediaHandle
{
	WallClock* wallclock;

	// handler of the events
	unique_ptr<EventHandler> event_handler;


	void mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer);
	void postHandleEvent(sf::Event& e);
	void preHandleEvent(sf::Event& e);
public:
	unique_ptr<EventMapper> event_mapper;

	sf::ContextSettings contextSettings;
	sf::Window window;
	Operator() = default;
	Operator(WallClock& wallclock, std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings);


	void operateSimulation(IGameMode* simulation);
	void disconnectSimulation();

	void createWindow(std::string name,  int reswidth, int resheight, IMediaHandle::ContextSettings& settings);
	void pollEvents();
	void setContextToMyThread();
	void display();

	void render();
};

#endif /* OPERATOR_HPP */

