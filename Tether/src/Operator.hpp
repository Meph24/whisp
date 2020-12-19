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

#include "Window.hpp"
#include "SFMLWindow.hpp"
#include "InputDeviceConfigurator.hpp"

#include <utility>

using glm::vec2;
using std::pair;
using std::unique_ptr;

class Operator
{
protected:
	sf::ContextSettings contextSettings;
public:
	Window* window;
	

	Operator(	
				std::string name, 
				int reswidth, 
				int resheight
				);


	void render();
	void display();
	void setContextToMyThread();
	
	virtual void pollEvents() = 0;
	virtual void operateSimulation(IGameMode* simulation) = 0;
	virtual void disconnectSimulation() = 0;
};


class LocalOperator : public Operator, public InputDeviceConfigurator
{
	MouseMode mousemode;
public:
	virtual vec2 turnSensitivity() const;
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

#endif /* OPERATOR_HPP */

