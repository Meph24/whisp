#include "MainApp.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "Cfg.hpp"
#include "CfgIO.hpp"

#include "EventHandler.h"

#include "Simulation_World.h"
#include "Zombie_World.h"

#define PHYSICS_MAX_TICKLENGTH 20000

unordered_map<string, unsigned int> Main::command_line_argument_flags = 
{  
	{"remote-control-sender", 2},
	{"remote-control-receiver", 0}	
};

Main::Main(int argc, char** argv)
	: commandline_argument_interpreter( CommandLineArgumentParser(command_line_argument_flags).parse(argc, argv) )
	, clock(base_clock)
{
	CfgIO cfgio ( "./res/config.txt" );
	cfg = cfgio.get();

	switch( commandline_argument_interpreter.execution_mode )
	{
		default:
		case CommandLineArgumentInterpreter::ExecutionMode::Default:
			app = std::make_unique<DefaultApp>(clock, cfg);
		break;
		case CommandLineArgumentInterpreter::ExecutionMode::RemoteControlSender:
		{
			IPv4Address addr(commandline_argument_interpreter.args["remote-control-sender"][0]); 
			Port port = std::stoi(commandline_argument_interpreter.args["remote-control-sender"][1]);
			app = std::make_unique<RemoteControlSenderApp>(clock, cfg, addr, port);
		}
		break;
		case CommandLineArgumentInterpreter::ExecutionMode::RemoteControlReceiver:
			app = std::make_unique<RemoteControlReceiverApp>(clock, cfg);
		break;
	}
}

void Main::run()
{
	if(!app) cout << "Not application selected! Aborting ...\n";
	app->run();
}

App::App(WallClock& wallclock, Cfg& cfg) 
		: wallclock(wallclock)
		, cfg(cfg) 
		{}

void App::run()
{
		if(!op || !sim) 
		{
			throw std::runtime_error(string(" App: Failure in application initialization <sim:") + string((sim ? "ok" : "missing")) + string("|op:") + string(op? "ok" : "missing") + string(">\n"));
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

		while (op->window->isOpen())
		{
			//render
			sim->loop();

			op->render();
			sim->doGraphics(sim->clock.now());
			op->display();

			//handle events
			op->pollEvents();	
		}
}

DefaultApp::DefaultApp( WallClock& wallclock, Cfg& cfg )
	: App(wallclock, cfg)
	, local_operator(cfg, "Dwengine", *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"))
{
	cout << "Initializing Default Application !\n";

	op = &local_operator;

	int zombie_mode = *cfg.getInt("test", "zombie_mode");
	if(zombie_mode) 
	{
		sim = std::make_unique<Zombie_World>(wallclock, &op->window->getSFWindow());
		sim->input_status->clip = true;
	}
	else 
	{
		sim = std::make_unique<Simulation_World>(wallclock, &op->window->getSFWindow());
	}

	op->operateSimulation(sim.get());
}

RemoteControlReceiverApp::RemoteControlReceiverApp(WallClock& wallclock, Cfg& cfg)
	: App(wallclock, cfg)
	, rc_operator( "Dwengine - remote controlled",  *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY") )
{
		cout << "Initializing RemoteControlReceiver Application !\n";
		op = &rc_operator;

		int zombie_mode = *cfg.getInt("test", "zombie_mode");
		if(zombie_mode) 
		{
			sim = std::make_unique<Zombie_World>(wallclock, &op->window->getSFWindow());
			sim->input_status->clip = true;
		}
		else 
		{
			sim = std::make_unique<Simulation_World>(wallclock, &op->window->getSFWindow());
		}
		op->operateSimulation( sim.get() );
}

RemoteControlSenderApp::RemoteControlSenderApp(WallClock& wallclock, Cfg& cfg, IPv4Address& addr, Port port)
		: App(wallclock, cfg)
		, rc_sender(wallclock, cfg)
{
	rc_sender.setMouseMode( InputDeviceConfigurator::MouseMode::diff );
	cout << "Initializing RemoteControlSender Application !\n";
	rc_sender.window.setPos(sf::Vector2i(0,0));
	rc_sender.tunein(addr, port);
}

void RemoteControlSenderApp::run()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	while(rc_sender.window.isOpen())
	{
		glClear( GL_COLOR_BUFFER_BIT );
		rc_sender.window.display();
		rc_sender.processEvents();
		rc_sender.sync();
	}
}