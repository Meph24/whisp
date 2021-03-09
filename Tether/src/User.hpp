#ifndef USER_HPP
#     define USER_HPP

#include <SFML/Window.hpp>
#include "EventMapper.hpp"
#include "WallClock.hpp"

#include <memory>
#include "Cfg.hpp"

#include <glm/vec2.hpp>

#include "SFMLWindow.hpp"
#include "InputDeviceConfigurator.hpp"
#include "Perspective.hpp"

#include <utility>

using glm::vec2;
using std::pair;
using std::unique_ptr;

class Simulation;

class SimulationUser
{
public:
	Simulation* simulation = nullptr;
	SimulationInputStatusSet input_status;
	
	EntityPlayer* avatar();
	virtual void operateSimulation(Simulation& simulation) = 0;
	virtual void disconnectSimulation() = 0;
};

struct LocalUser : public SimulationUser , public InputDeviceConfigurator
{
	sf::ContextSettings context_settings;
	SFMLWindow window;

	//impl User
	unique_ptr<Perspective> perspective;
	void operateSimulation(Simulation& simulation);
	void disconnectSimulation();

	unique_ptr<EventMapper> event_mapper;
	EventSource* event_source;
	void pollEvents();

	map<InputDeviceConfigurator::MouseMode, vector<pair<int, EventMapping>>> mouse_mode_mappings;
	MouseMode mousemode;
	virtual void setMouseMode( MouseMode mode );
	virtual MouseMode mouseMode() const;
	vec2 turnSensitivity() const;

	LocalUser(	
				const Cfg& cfg, 
				string window_name,
				int reswidth, 
				int resheight 
	);

	LocalUser(const LocalUser&) = delete;
	LocalUser& operator=(const LocalUser&) = delete;
	LocalUser(LocalUser&&) = delete;
	LocalUser& operator=(LocalUser&&) = delete;

	void draw();
};

#endif /* USER_HPP */

