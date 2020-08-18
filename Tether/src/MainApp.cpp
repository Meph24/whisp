#include "MainApp.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "Cfg.hpp"
#include "CfgIO.hpp"

#include "EventHandler.h"
#include "IMediaHandle.h"

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

	sfmlHandle.createWindow("Test", x,y, settings);
}

void MainApp::tick(int us)
{
	counter += us*0.000001f;
	//do stuff
}

#include "IGameMode.h"
IGameMode* world;

void MainApp::run()
{
	sfmlHandle.window.setActive();

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


	sf::Clock clock;

	world->init();
	srand(time(0));

	while (sfmlHandle.window.isOpen())
	{
		//render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		world->loop();
		sfmlHandle.display();

		//tick
		sf::Time elapsed = clock.restart();
		int us = elapsed.asMicroseconds();
		if (us > PHYSICS_MAX_TICKLENGTH) us = PHYSICS_MAX_TICKLENGTH;
		tick(us);

		//handle events
		sfmlHandle.pollEvents();	
	}

}

MainApp::~MainApp()
{
}
