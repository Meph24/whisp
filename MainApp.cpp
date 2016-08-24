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



MainApp::MainApp()
{
	IMediaHandle::ContextSettings settings;
	settings.depth = 24;
	settings.stencil = 8;
	settings.antialiasing = 0;
	settings.openglmajor = 3;
	settings.openglminor = 3;
	SFMLHandle.createWindow("Test", 800, 600, settings);
	graphics.init();
}
/*
std::ostream& operator<< (std::ostream& os, const MainApp& A)
{
	os << "OPENGL: " << std::endl;
	os << " VERSION: " << glGetString(GL_VERSION) << std::endl;
	os << " VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	os << " RENDERER: " << glGetString(GL_RENDERER) << std::endl << std::endl;

	//Check OpenGL Context Settings 
	sf::ContextSettings context = A.window.getSettings();
	os << "OpenGL Context: " << std::endl;
	os << " GL_Version: " << context.majorVersion << "." << context.minorVersion << std::endl;
	os << " Depth Bits: " << context.depthBits << std::endl;
	os << " Stencil Bits " << context.stencilBits << std::endl;
	os << " Anti-aliasing: " << context.antialiasingLevel << std::endl << std::endl;

	return os;
}
*/


void MainApp::tick(int us)
{
	counter += us*0.000002f;
}

void MainApp::run()
{
	
	sf::Clock clock;

	while (SFMLHandle.window.isOpen())
	{
		/*
		//tick
		sf::Time elapsed = clock.restart();
		int us = elapsed.asMicroseconds();
		if (us > PHYSICS_MAX_TICKLENGTH) us = PHYSICS_MAX_TICKLENGTH;
		tick(us);
*/


		//render
		graphics.render();
		
		SFMLHandle.window.display();

		//handle events
		SFMLHandle.pollEvents();	
		
	}

}




MainApp::~MainApp()
{

}