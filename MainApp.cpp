#include "MainApp.h"
#include <GL\glew.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

#include "EventHandler.h"

#include <vector>



// error handling
#include <iostream>




#define PHYSICS_MAX_TICKLENGTH 20000



MainApp::MainApp()
{
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
	
	/*

	//old code
	
	std::vector<Vertex> vertices = {

		Vertex(glm::vec3(-2.0, 0.0, 0.0), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(0.0, 0.0, 3.0), glm::vec2(0.5, 0.0)),
		Vertex(glm::vec3(2.0, 0.0, 0.0), glm::vec2(1.0, 1.0))

	};
	

	std::vector<unsigned int> indices = { 0, 1, 2 };
	

	Mesh mesh(&(vertices[0]), vertices.size(), &(indices[0]), indices.size());
	


	Shader shader("./res/basicShader");
	Texture texture("./res/testTexture.jpg");
	Camera camera(glm::vec3(0, 1, -3), MEPH_GRAPHICS_FOV, (float)MEPH_GRAPHICS_WIDTH / (float)MEPH_GRAPHICS_HEIGHT, 0.01f, 1000.0f);

	Transform transform;

	*/



	sf::Keyboard::setVirtualKeyboardVisible(true);

	sf::Clock clock;

	while (oshandle.window.isOpen())
	{
		//tick
		sf::Time elapsed = clock.restart();
		int us = elapsed.asMicroseconds();
		if (us > PHYSICS_MAX_TICKLENGTH) us = PHYSICS_MAX_TICKLENGTH;
		tick(us);



		//render
		graphics.render();
		
		oshandle.window.display();

		//handle events
		oshandle.pollEvents();		
	}

}




MainApp::~MainApp()
{

}