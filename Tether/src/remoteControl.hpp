#ifndef REMOTECONTROL_HPP
#define REMOTECONTROL_HPP

#include "IPv4Address.hpp"
#include "MainApp.hpp"
#include "NetworkConnection.hpp"
#include "SFMLWindow.hpp"

using network::IPv4Address;
using network::Port;
using network::UdpSocket;

struct RemoteControlReceiverUser : public LocalUser
{
	size_t received;
	UdpSocket udpsocket;
	SimulationInputStatusSet current_receive;

	RemoteControlReceiverUser( Cfg& cfg, string window_name, int reswidth, int resheight, Port try_port );
	void operateSimulation(Simulation* simulation);
	void disconnectSimulation();

	void processNetworkInput();
};

struct RemoteControlSender : public InputDeviceConfigurator
{
	WallClock& clock;
	SFMLWindow window;

	EventMapper event_mapper;
	EventSource* event_source;

	SimulationInputStatusSet input_status;

	IPv4Address receiver_addr;
	Port receiver_port;
	UdpSocket udpsocket;

	MouseMode mousemode;

	WallClock::time_point last_synced;

	RemoteControlSender( WallClock& wallclock, Cfg& cfg);

	map<MouseMode, vector<pair<int, EventMapping>>> mouse_mode_mappings;

	void setMouseMode( MouseMode mode );
	MouseMode mouseMode() const;

	void operateRemote();
	void tunein(IPv4Address&, Port);
	void sync();
	void processEvents();
};

struct RemoteControlReceiverApp : public App
{
	WallClock& wallclock;
	Cfg& cfg;

	unique_ptr<Simulation> sim;
	RemoteControlReceiverUser user;

	RemoteControlReceiverApp(WallClock& wallclock, Cfg& cfg, Port port);
	void run();
};

struct RemoteControlSenderApp : public App
{
	RemoteControlSender rc_sender;

	RemoteControlSenderApp( WallClock& wallclock, Cfg& cfg, IPv4Address& addr, Port port = 55555 );
	void run();
};

#endif // REMOTECONTROL_HPP
