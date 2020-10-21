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

#include "InputDeviceConfigurator.hpp"

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

struct RemoteControlReceiverOperator : public Operator
{
	SimulationInputStatusSet* managed_status;

	SFMLWindow sfmlwindow;

	RemoteControlReceiverOperator( std::string name, int reswidth, int resheight );
	void operateSimulation(IGameMode* simulation) { managed_status = simulation->input_status.get(); }
	void disconnectSimulation(){ managed_status = nullptr; }
	void pollEvents() { /* noting to do here, done on sender, this call will hopefully be smth different in the future*/ }

};
struct RemoteControlSender : public InputDeviceConfigurator
{
	SFMLWindow window;
	EventHandler event_handler;
	EventMapper event_mapper;
	EventSource* event_source;

	SimulationInputStatusSet status_set;
	SimulationInputStatusSet* receiver_status_set;

	RemoteControlSender( const Cfg& cfg )
	: InputDeviceConfigurator(cfg)
	, window( 200, 200, "whisp-remote-control-at-someipidontknowrightnow", sf::Style::None, sf::ContextSettings())
	, event_mapper( status_set )
	, event_source( &window )
	, receiver_status_set( nullptr )
	{
		
	}

	vec2 turnSensitivity() const;
	map<MouseMode, vector<pair<int, EventMapping>>> mouse_mode_mappings;
	void setMouseMode( MouseMode mode );
	MouseMode mouseMode() const;

	void connect( RemoteControlReceiverOperator& rcro );

	void sync()
	{
		*receiver_status_set = status_set;
	}
};

#endif /* OPERATOR_HPP */

