#pragma once

#include <string>
#include <stdexcept>

#include "Operator.hpp"
#include <memory>
#include "WallClock.hpp"
#include "SFMLClock.hpp"

using std::unique_ptr;

#include "commandline.hpp"

struct App
{
	virtual void run() = 0;
};

struct DefaultApp : public App
{
	WallClock& wallclock;
	Cfg& cfg;

	unique_ptr<IGameMode> sim;
	LocalOperator op;

	DefaultApp( WallClock& wallclock, Cfg& cfg );
	void run();
};

#include "IPv4Address.hpp"

struct RemoteControlReceiverApp : public App
{
	WallClock& wallclock;
	Cfg& cfg;

	unique_ptr<IGameMode> sim;
	RemoteControlReceiverOperator op;

	RemoteControlReceiverApp(WallClock& wallclock, Cfg& cfg, Port port);
	void run();
};

struct RemoteControlSenderApp : public App
{
	RemoteControlSender rc_sender;

	RemoteControlSenderApp( WallClock& wallclock, Cfg& cfg, IPv4Address& addr, Port port = 55555 );
	void run();
};

/*
struct ServerApp : public App
{
	SimulationServer simserver;
	ServerApp( WallClock& wallclock, Cfg& cfg, Port port );
	void run();
};
*/

class Main
{
	static unordered_map<string, unsigned int> command_line_argument_flags;
	CommandLineArgumentInterpreter commandline_argument_interpreter;

	SFMLClock base_clock;
	WallClock clock;
	Cfg cfg;

	unique_ptr<App> app;
	
public:
	Main(int argc, char** argv);
	void run();
};
