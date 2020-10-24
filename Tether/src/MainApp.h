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
	WallClock& wallclock;
	Cfg& cfg;

	App(WallClock& wallclock, Cfg& cfg);

	unique_ptr<IGameMode> sim;
	Operator* op;

	virtual void run();
};

struct DefaultApp : public App
{
	LocalOperator local_operator;
	DefaultApp( WallClock& wallclock, Cfg& cfg );
};

#include "IPv4Address.hpp"

struct RemoteControlReceiverApp : public App
{
	RemoteControlReceiverOperator rc_operator;
	RemoteControlReceiverApp(WallClock& wallclock, Cfg& cfg, Port port);
};

struct RemoteControlSenderApp : public App
{
	RemoteControlSender rc_sender;
	RemoteControlSenderApp( WallClock& wallclock, Cfg& cfg, IPv4Address& addr, Port port = 55555 );
	void run();
};

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
