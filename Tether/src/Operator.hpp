#ifndef OPERATOR_HPP
#     define OPERATOR_HPP

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

#include "Window.hpp"
#include "SFMLWindow.hpp"

class Operator
{
protected:
	const Cfg& cfg;
	sf::ContextSettings contextSettings;
	vec2 turn_sensitivity;
public:
	Window* window;
	enum MouseMode
	{
		pointer, diff, NUM_MOUSEMODES
	}; //make private

	virtual void setMouseMode( MouseMode mode ) = 0;
	virtual MouseMode mouseMode() const = 0;
	virtual vec2 turnSensitivity() const = 0;

	Operator(	
				const Cfg& cfg, 
				std::string name, 
				int reswidth, 
				int resheight
				);

	virtual void pollEvents() = 0;
	void display();

	void setContextToMyThread();
	
	void render();
	virtual void operateSimulation(IGameMode* simulation) = 0;
	virtual void disconnectSimulation() = 0;
};

class LocalOperator : public Operator
{
	MouseMode mousemode;
public:
	vec2 turnSensitivity() const
	{ 
		float modifier = 1.0f;
		if( event_mapper && event_mapper->managed_stati->zoom ) modifier = 1.0f / 8; 
		return turn_sensitivity * modifier; 
	}

	map<MouseMode, vector<pair<int, EventMapping>>> mouse_mode_mappings;
	void setMouseMode( MouseMode mode );
	MouseMode mouseMode() const;

	SFMLWindow sfmlwindow;
	EventHandler event_handler;
	unique_ptr<EventMapper> event_mapper;
	EventSource* event_source;
	void operateSimulation(IGameMode* simulation);
	void disconnectSimulation();
	void pollEvents();

	void preHandleEvent(sf::Event& e);
	void postHandleEvent(sf::Event& e);

	LocalOperator(	
				const Cfg& cfg, 
				std::string name, 
				int reswidth, 
				int resheight 
	);
};

/*
struct RemoteControlReceiverOperator : public Operator
{
	RemoteControlReceiverOperator() 
};
*/

#endif /* OPERATOR_HPP */

