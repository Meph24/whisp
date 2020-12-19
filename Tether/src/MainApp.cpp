#include "MainApp.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "Cfg.hpp"
#include "CfgIO.hpp"

#include "EventHandler.h"

#include "Simulation_World.h"
#include "Zombie_World.h"
#include "remoteControl.hpp"

#define PHYSICS_MAX_TICKLENGTH 20000

unordered_map<string, unsigned int> Main::command_line_argument_flags = 
{  
	{"remote-control-sender", 0},
	{"remote-control-receiver", 0},
	{"addr", 1},
	{"port", 1}
};

Main::Main(int argc, char** argv)
	: commandline_argument_interpreter( CommandLineArgumentParser(command_line_argument_flags).parse(argc, argv) )
	, clock(base_clock)
{
	CfgIO cfgio ( "./res/config.txt" );
	cfg = cfgio.get();

	Port port = (*cfg.getInt("network", "default_port_remote_control"));
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
		case CommandLineArgumentInterpreter::ExecutionMode::RemoteControlSender:
		{
			IPv4Address addr ( *cfg.getStr( "network", "default_ip_remote_control_sender" ) );
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
	, op(cfg, "Dwengine", *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"))
{
	cout << "Initializing Default Application !\n";


	int zombie_mode = *cfg.getInt("test", "zombie_mode");
	if(zombie_mode) 
	{
		sim = std::make_unique<Zombie_World>(wallclock, &op.window->getSFWindow());
		sim->input_status->clip = true;
	}
	else 
	{
		sim = std::make_unique<Simulation_World>(wallclock, &op.window->getSFWindow());
	}

	op.operateSimulation(sim.get());
}

void DefaultApp::run()
{
	cout << "Initializing Default Application!\n";
	if(!sim) 
	{
		throw std::runtime_error( "App: Failure in application initialization < No Simulation to simulate >!" );
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.25f, 0.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glAlphaFunc(GL_GREATER, 0.02f);
	glEnable(GL_ALPHA_TEST);

	sim->init();
	srand(time(0));

	while (op.window->isOpen())
	{
		//render
		sim->loop();

		op.render();
		sim->doGraphics(sim->clock.now());
		op.display();

		//handle events
		op.pollEvents();	
	}
}
