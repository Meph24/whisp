#include "MainApp.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "Cfg.hpp"
#include "CfgIO.hpp"

#include "EventHandler.h"
#include "IMediaHandle.h"

#include "Simulation_World.h"
#include "Zombie_World.h"

#define PHYSICS_MAX_TICKLENGTH 20000

MainApp::MainApp()
	: clock(base_clock)
{
	IMediaHandle::ContextSettings settings;
	settings.depth = 24;
	settings.stencil = 8;
	settings.antialiasing = 1;
	settings.openglmajor = 2;
	settings.openglminor = 0;

	//void * pointer=malloc(1024*1024);//This worked
	//Cfg * cfg=new (pointer) Cfg({ "./res/config.txt" });
    //
	//int x=*(cfg->getint("graphics", "resolutionX"));
	//int y=*(cfg->getint("graphics", "resolutionY"));
	//free(pointer);
	CfgIO cfgio ( "./res/config.txt" );
	Cfg cfg = cfgio.get();

	int x=*cfg.getInt("graphics", "resolutionX");
	int y=*cfg.getInt("graphics", "resolutionY");


	op = std::make_unique<LocalOperator>(clock, "Dwengine", x, y, settings);

	int zombie_mode = *cfg.getInt("test", "zombie_mode");
	if(zombie_mode) 
	{
		sim.reset(new Zombie_World(clock, &op->window()));
		sim->input_status->clip = true;
	}
	else 
	{
		sim.reset(new Simulation_World(clock, &op->window()));
	}

	op->operateSimulation(sim.get());
}

void MainApp::run()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.25f, 0.f);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glAlphaFunc(GL_GREATER, 0.02f);
	glEnable(GL_ALPHA_TEST);

	sim->init();
	srand(time(0));

	while (op->window().isOpen())
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
