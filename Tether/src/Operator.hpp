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
#include "Cfg.hpp"

#include <glm/vec2.hpp>
using glm::vec2;

#include <utility>
using std::pair;

using std::unique_ptr;

class Operator : public IMediaHandle
{
protected:
	WallClock* wallclock;
	const Cfg& cfg;
	unique_ptr<EventHandler> event_handler;


	sf::ContextSettings contextSettings;
	vec2 turn_sensitivity;
public:
	enum MouseMode
	{
		pointer, diff, NUM_MOUSEMODES
	} mousemode ; //make private

	map<MouseMode, vector<pair<int, EventMapping>>> mouse_mode_mappings;
	void setMouseMode( MouseMode mode );

	sf::Window window;

	vec2 turnSensitivity() const 
	{ 
		float modifier = 1.0f;
		if( event_mapper && event_mapper->managed_stati->zoom ) modifier = 1.0f / 8; 
		return turn_sensitivity * modifier; 
	}

	unique_ptr<EventMapper> event_mapper;

	Operator(	WallClock& wallclock, 
				const Cfg& cfg, 
				std::string name, 
				int reswidth, 
				int resheight, 
				IMediaHandle::ContextSettings& settings	);


	virtual void operateSimulation(IGameMode* simulation) = 0;
	virtual void disconnectSimulation() = 0;

	virtual void render() = 0;

	void mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer);
	void postHandleEvent(sf::Event& e);
	void preHandleEvent(sf::Event& e);
	void pollEvents();
	void display();

	void createWindow(std::string name,  int reswidth, int resheight, IMediaHandle::ContextSettings& settings);
	void setContextToMyThread();
};

struct LocalOperator : public Operator
{

	LocalOperator() = default;
	LocalOperator(WallClock& wallclock, const Cfg& cfg, std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings);

	
	void operateSimulation(IGameMode* simulation);
	void disconnectSimulation();
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

