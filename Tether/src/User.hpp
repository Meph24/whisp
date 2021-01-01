#ifndef USER_HPP
#     define USER_HPP

#include <SFML/Window.hpp>
#include "EventHandler.h"
#include "EventMapper.hpp"
#include "Buffer.h"
#include "WallClock.hpp"


#include <memory>
#include "Cfg.hpp"

#include <glm/vec2.hpp>

#include "Window.hpp"
#include "SFMLWindow.hpp"
#include "InputDeviceConfigurator.hpp"
#include "Perspective.hpp"

#include <utility>

using glm::vec2;
using std::pair;
using std::unique_ptr;

class Simulation;

class User
{
protected:
	sf::ContextSettings contextSettings;
public:
	string name;
	Window* window;
	Simulation* simulation;
	unique_ptr<Perspective> perspective;

	SimulationInputStatusSet input_status;

	User(	std::string name, 
			int reswidth, 
			int resheight
			);

	void draw();
	void display();
	void setContextToMyThread();
	
	virtual void pollEvents() = 0;
	virtual void operateSimulation(Simulation* simulation) = 0;
	virtual void disconnectSimulation() = 0;
};


class LocalUser : public User, public InputDeviceConfigurator
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

	void operateSimulation(Simulation* simulation);
	void disconnectSimulation();
	void pollEvents();

	void preHandleEvent(sf::Event& e);
	void postHandleEvent(sf::Event& e);

	LocalUser(	
				const Cfg& cfg, 
				std::string name, 
				int reswidth, 
				int resheight 
	);

	LocalUser(const LocalUser&) = delete;
	LocalUser& operator=(const LocalUser&) = delete;
};

#endif /* USER_HPP */

