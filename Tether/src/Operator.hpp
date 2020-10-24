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

#include "IPv4Address.hpp"
#include "NetworkConnection.hpp"
using network::IPv4Address;
using network::Port;
using network::UdpSocket;

struct RemoteControlReceiverOperator : public Operator
{
	SimulationInputStatusSet* managed_status;

	SFMLWindow sfmlwindow;

	SimulationInputStatusSet current_receive;
	size_t received;
	UdpSocket udpsocket;

	RemoteControlReceiverOperator( std::string name, int reswidth, int resheight );
	void operateSimulation(IGameMode* simulation) 
	{ 
		managed_status = simulation->input_status.get();
	}
	void disconnectSimulation(){ managed_status = nullptr; }
	void pollEvents() 
	{ 
		if(!managed_status) return;
		sf::IpAddress addr; Port port;
		size_t this_received = 1;
		char * data = (char*)&current_receive + received;
		while(this_received)
		{
			this_received = 0;
			udpsocket.receive(data, sizeof(SimulationInputStatusSet) - received, this_received, addr, port);
			if(!this_received) break;
			received += this_received;
			if(received == sizeof(SimulationInputStatusSet))
			{
				if(managed_status->timestamp < current_receive.timestamp)
				{
					*managed_status = current_receive;
				}
				received = 0;
			}
		}

		sf::Event e;
		while(sfmlwindow.pollEvent(e))
		{
			switch (e.type)
			{
				case sf::Event::EventType::Closed:
					disconnectSimulation();
					window->close();
				break;

				default: break;//warning suppression
			}
		}
	}
};

struct RemoteControlSender : public InputDeviceConfigurator
{
	WallClock& clock;
	SFMLWindow window;

	EventHandler event_handler;
	EventMapper event_mapper;
	EventSource* event_source;

	SimulationInputStatusSet status_set;

	IPv4Address receiver_addr;
	Port receiver_port;
	UdpSocket udpsocket;

	MouseMode mousemode;

	WallClock::time_point last_synced;

	RemoteControlSender( WallClock& wallclock, Cfg& cfg);

	vec2 turnSensitivity() const;
	map<MouseMode, vector<pair<int, EventMapping>>> mouse_mode_mappings;

	void setMouseMode( MouseMode mode );
	MouseMode mouseMode() const;

	void operateRemote();
	void tunein(IPv4Address&, Port);
	void sync();
	void processEvents();
};

#endif /* OPERATOR_HPP */

