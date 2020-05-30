#pragma once

#include "SFMLHandle.h"
#include "Graphics.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "Cfg.hpp"
#include "CfgIO.hpp"

#include "EventHandler.h"
#include "IMediaHandle.h"

#define PHYSICS_MAX_TICKLENGTH 20000


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
	}

	~MainApp() = default;

	void run()
	{
		sf::Clock clock;

		graphics.start();
		while (sfmlHandle.window.isOpen())
		{
			
			//tick
			sf::Time elapsed = clock.restart();
			int us = elapsed.asMicroseconds();
			if (us > PHYSICS_MAX_TICKLENGTH) us = PHYSICS_MAX_TICKLENGTH;

			sf::Time t = sf::microseconds(1000);
			sf::sleep(t);
			

			//handle events
			sfmlHandle.pollEvents();	
		}

	}

	

};
