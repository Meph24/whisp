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

class Operator : public IMediaHandle
{
	WallClock* wallclock;
public:
	Operator(WallClock& wallclock) : wallclock(&wallclock) {}
	virtual void operateSimulation(IGameMode* simulation) = 0;
	virtual void disconnectSimulation() = 0;

	virtual sf::Window& window() = 0;

	virtual void render() = 0;
};

struct LocalOperator : public Operator
{
	// handler of the events
	unique_ptr<EventHandler> event_handler;

	void mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer);
	void postHandleEvent(sf::Event& e);
	void preHandleEvent(sf::Event& e);
public:
	unique_ptr<EventMapper> event_mapper;

	sf::ContextSettings contextSettings;
	sf::Window m_window;
	LocalOperator() = default;
	LocalOperator(WallClock& wallclock, std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings);


	void operateSimulation(IGameMode* simulation);
	void disconnectSimulation();

	void createWindow(std::string name,  int reswidth, int resheight, IMediaHandle::ContextSettings& settings);
	sf::Window& window();
	void pollEvents();
	void setContextToMyThread();
	void display();

	void render();
};

/*
namespace RemoteOperator
{
	class Server : public Operator
	{
		void operateSimulation(IGameMode* simulation);
		void disconnectSimulation();

		void createWindow(std::string name,  int reswidth, int resheight, IMediaHandle::ContextSettings& settings);
		void pollEvents();
		void setContextToMyThread();
		void display();

		void render();
	};
	class Client
	{
		// handler of the events
		unique_ptr<EventHandler> event_handler;

		void mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer);
		void postHandleEvent(sf::Event& e);
		void preHandleEvent(sf::Event& e);
	};
} // namespace RemoteOperator
*/

#endif /* OPERATOR_HPP */

