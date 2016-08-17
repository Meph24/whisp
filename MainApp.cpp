#include "MainApp.h"
#include <GL\glew.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

#include "EventHandler.h"
#include "SFMLEventMapper.h"



// error handling
#include <iostream>

#define MEPH_GRAPHICS_FOV 70.0f
#define MEPH_GRAPHICS_WIDTH 800
#define MEPH_GRAPHICS_HEIGHT 600


#define PHYSICS_TICKLENGTH 100000



MainApp::MainApp() :
contextSettings(24, 8, 0, 3, 3),
window(sf::VideoMode(800, 600), "This is a test !", sf::Style::Default, contextSettings)
{

	//Output OpenGL stats
	

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 255.f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

}

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

void MainApp::windowClear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}

void MainApp::setClearParameters(float r, float g, float b, float a, float depth)
{
	glClearDepth(depth);
	glClearColor(r, g, b, a);
}

void MainApp::windowClear(float r, float g, float b, float a, float depth)
{
	setClearParameters(r, g , b, a, depth);
	windowClear();
}



void MainApp::run()
{
	
	//Game Loop
	
	

	Vertex vertices[] = {

		Vertex(glm::vec3(-2.0, 0.0, 0.0), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(0.0, 0.0, 3.0), glm::vec2(0.5, 0.0)),
		Vertex(glm::vec3(2.0, 0.0, 0.0), glm::vec2(1.0, 1.0))


	};

	unsigned int indices[] = { 0, 1, 2};

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

	Shader shader("./res/basicShader");

	Texture texture("./res/testTexture.jpg");

	Camera camera(glm::vec3(0, 1, -3), MEPH_GRAPHICS_FOV, (float)MEPH_GRAPHICS_WIDTH / (float)MEPH_GRAPHICS_HEIGHT, 0.01f, 1000.0f);

	Transform transform;

	counter = 0; //test variable


	EventHandler eventHandler;
	SFMLEventMapper EventMapper(eventHandler, window);

	
	
	while (window.isOpen())
	{
		
		counter += 0.01f;

		EventMapper.handleEvents();


		transform.pos.z = sinf(counter);
			shader.Bind();
			texture.bind();
			shader.Update(transform, camera);
			windowClear();
			mesh.draw();
			
			window.display();		
	}




}

MainApp::~MainApp()
{

}
