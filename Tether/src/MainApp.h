#pragma once

#include "SFMLHandle.h"
#include "Graphics.h"

#include <iostream>
#include <memory>
#include <vector>

#include <GL/glew.h>

#include "Cfg.hpp"
#include "CfgIO.hpp"

#include "EventHandler.h"
#include "IMediaHandle.h"

#include "IGameMode.h"
#include "Simulation_World.h"

#define PHYSICS_MAX_TICKLENGTH 20000

using std::unique_ptr;

class MainApp
{
	/*
	//	Media Handle Instance, encapsulation of the Media Layer
	*/
	SFMLHandle sfmlHandle;

	/*
	//	Rendering component
	*/
	Graphics graphics;

	unique_ptr<IGameMode> gamemode;

public:
	MainApp()
		:graphics(sfmlHandle)
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
		graphics.init();

		gamemode = std::unique_ptr<IGameMode>(new Simulation_World(&sfmlHandle.window));


		gamemode->loadStandardTex();
		srand(time(0));
	}

	void run()
	{
		sf::Clock clock;



		while (sfmlHandle.window.isOpen())
		{
			graphics.clear();
			gamemode->loop();
			sfmlHandle.display();	

			//handle events
			sfmlHandle.pollEvents();	
		}

	}

	

};
