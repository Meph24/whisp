#include "MainApp.h"
#include <GL/glew.h>

#include "EventHandler.h"

#include <vector>

#include "IMediaHandle.h"

// error handling
#include <iostream>

#include "Cfg.h"


#define PHYSICS_MAX_TICKLENGTH 20000



MainApp::MainApp():
graphics(sfmlHandle)
{
	IMediaHandle::ContextSettings settings;
	settings.depth = 24;
	settings.stencil = 8;
	settings.antialiasing = 0;
	settings.openglmajor = 1;
	settings.openglminor = 2;

	//void * pointer=malloc(1024*1024);//This worked
	//Cfg * cfg=new (pointer) Cfg({ "./res/config.txt" });
    //
	//int x=*(cfg->getint("graphics", "resolutionX"));
	//int y=*(cfg->getint("graphics", "resolutionY"));
	//free(pointer);
	Cfg cfg({ "./res/config.txt" });

	int x=*cfg.getint("graphics", "resolutionX");
	int y=*cfg.getint("graphics", "resolutionY");

	


	sfmlHandle.createWindow("Test", x,y, settings);
}

void MainApp::tick(int us)
{
	counter += us*0.000001f;
	//do stuff
}

void MainApp::run()
{
	
	sf::Clock clock;


	graphics.start();
	while (sfmlHandle.window.isOpen())
	{
		
		//tick
		sf::Time elapsed = clock.restart();
		int us = elapsed.asMicroseconds();
		if (us > PHYSICS_MAX_TICKLENGTH) us = PHYSICS_MAX_TICKLENGTH;
		tick(us);

		sf::Time t = sf::microseconds(1000);
		sf::sleep(t);
		

		//handle events
		sfmlHandle.pollEvents();	
		
	}

}




MainApp::~MainApp()
{

}
