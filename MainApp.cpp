#include "MainApp.h"
#include <GL\glew.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

#include "EventHandler.h"

#include <vector>

#include "IMediaHandle.h"

// error handling
#include <iostream>




#define PHYSICS_MAX_TICKLENGTH 20000



MainApp::MainApp():
graphics(SFMLHandle)
{
	IMediaHandle::ContextSettings settings;
	settings.depth = 24;
	settings.stencil = 8;
	settings.antialiasing = 0;
	settings.openglmajor = 3;
	settings.openglminor = 3;
	SFMLHandle.createWindow("Test", 800, 600, settings);
}

void MainApp::tick(int us)
{
	counter += us*0.000002f;
}

void MainApp::run()
{
	
	sf::Clock clock;


	graphics.start();
	while (SFMLHandle.window.isOpen())
	{
		
		//tick
		sf::Time elapsed = clock.restart();
		int us = elapsed.asMicroseconds();
		if (us > PHYSICS_MAX_TICKLENGTH) us = PHYSICS_MAX_TICKLENGTH;
		tick(us);
		
		

		//handle events
		SFMLHandle.pollEvents();	
		
	}

}




MainApp::~MainApp()
{

}