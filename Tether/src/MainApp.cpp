#include "MainApp.hpp"

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "Cfg.hpp"
#include "CfgIO.hpp"

#include "Simulation_World.h"
#include "Zombie_World.h"
#include "remoteControl.hpp"
#include "synchronized.hpp"

using std::cout;

#define PHYSICS_MAX_TICKLENGTH 20000

unordered_map<string, unsigned int> Main::command_line_argument_flags = 
{  
	{"remote-control-sender", 0},
	{"remote-control-receiver", 0},
	{"host", 0},
	{"client", 0},
	{"addr", 1},
	{"port", 1}
};

Main::Main(int argc, char** argv)
	: commandline_argument_interpreter( CommandLineArgumentParser(command_line_argument_flags).parse(argc, argv) )
	, clock(base_clock)
{
	CfgIO cfgio ( "./res/config.txt" );
	cfg = cfgio.get();

	srand(time(0));

	Port port = (*cfg.getInt("network", "default_connect_port"));
	if(port < 0 || port > 65535)
		throw std::runtime_error( " Port loaded from cfg " + std::to_string(port) + "is not a port!\n" );
	if(commandline_argument_interpreter.args.find("port") != commandline_argument_interpreter.args.end() )
	{
		port = std::stoi(commandline_argument_interpreter.args["port"][0]);
		if(port < 0 || port > 65535)
			throw std::runtime_error( " Port inserted as commandline argument " + std::to_string(port) + "is not a port!\n" );
	}

	switch( commandline_argument_interpreter.execution_mode )
	{
		default:
		case CommandLineArgumentInterpreter::ExecutionMode::Default:
			app = std::make_unique<DefaultApp>(clock, cfg);
		break;
		case CommandLineArgumentInterpreter::ExecutionMode::Host:
			app = std::make_unique<HostApp>(clock, cfg, port);
		break;
		case CommandLineArgumentInterpreter::ExecutionMode::Client:
		{
			IPv4Address addr ( *cfg.getStr( "network", "default_connect_ip" ) );
			if( commandline_argument_interpreter.args.find("addr") != commandline_argument_interpreter.args.end() )
				addr = IPv4Address(commandline_argument_interpreter.args["addr"][0]);

			app = std::make_unique<ClientApp>(clock, cfg, addr, port);
		}
		break;
		case CommandLineArgumentInterpreter::ExecutionMode::RemoteControlSender:
		{
			IPv4Address addr ( *cfg.getStr( "network", "default_connect_ip" ) );
			if( commandline_argument_interpreter.args.find("addr") != commandline_argument_interpreter.args.end() )
				addr = IPv4Address(commandline_argument_interpreter.args["addr"][0]);

			app = std::make_unique<RemoteControlSenderApp>(clock, cfg, addr, port);
		}
		break;
		case CommandLineArgumentInterpreter::ExecutionMode::RemoteControlReceiver:
			app = std::make_unique<RemoteControlReceiverApp>(clock, cfg, port);
		break;
	}
}

void Main::run()
{
	if(!app) cout << "Not application selected! Aborting ...\n";
	app->run();
}

DefaultApp::DefaultApp( WallClock& wallclock, Cfg& cfg )
	: wallclock(wallclock)
	, cfg(cfg)
	, local_user(cfg, *cfg.getStr("general", "application_name"),*cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"))
{
	cout << "Initializing Default Application !\n";

	int zombie_mode = *cfg.getInt("test", "zombie_mode");
	if(zombie_mode) 
	{
		sim = std::make_unique<Zombie_World>(wallclock, cfg);
		local_user.input_status.clip = true;
	}
	else 
	{
		sim = std::make_unique<Simulation_World>(wallclock, cfg);
	}

	local_user.operateSimulation(*sim);
}

void DefaultApp::run()
{
	cout << "Running Default Application!\n";
	if(!sim) 
	{
		throw std::runtime_error( "App: Failure in application initialization < No Simulation to simulate >!" );
	}

	sim->init();

	local_user.perspective = std::make_unique<Perspective>(local_user, *sim);

	while (local_user.window.isOpen())
	{
		//render
		sim->step();

		local_user.perspective->setAvatar(sim->userAvatar(&local_user));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		local_user.draw();
		local_user.window.display();

		//handle events
		local_user.pollEvents();
	}
}
