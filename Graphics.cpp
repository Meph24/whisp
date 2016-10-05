#include "Graphics.h"
#include <GL\glew.h>
#include <iostream>

#define MEPH_GRAPHICS_FOV 70.0f
#define MEPH_GRAPHICS_WIDTH 800
#define MEPH_GRAPHICS_HEIGHT 600

Graphics::Graphics(IMediaHandle& mediaHandle) :
mediaHandle(mediaHandle)
{}

Graphics::Graphics(IMediaHandle& mediaHandle, Graphics::settings settings) :
Graphics(mediaHandle)
{
	init();
}

Graphics::~Graphics()
{
	stop();

	delete mesh;
	delete shader;
	delete texture;
	delete camera;
	delete transform;
}

void Graphics::init()
{	
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.1f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);


	vertices = {

		Vertex(glm::vec3(-2.0, 0.0, 0.0), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(0.0, 0.0, 3.0), glm::vec2(0.5, 0.0)),
		Vertex(glm::vec3(2.0, 0.0, 0.0), glm::vec2(1.0, 1.0))

	};

	indices = { 0, 1, 2 };

	mesh = new Mesh(&(vertices[0]), vertices.size(), &(indices[0]), indices.size());

	shader = new Shader("./res/basicShader");

	texture = new Texture("./res/testTexture.jpg");

	camera = new Camera(glm::vec3(0, 1, -3), MEPH_GRAPHICS_FOV, (float)MEPH_GRAPHICS_WIDTH / (float)MEPH_GRAPHICS_HEIGHT, 0.01f, 1000.0f);

	transform = new Transform();
}

void Graphics::start()
{
	if (thread_isOn)
	{
		stop();
	}
	thread_isOn = true;
	graphicThread = std::thread(&Graphics::graphicMain, this);
}

void Graphics::stop()
{
	if (thread_isOn)
	{
		thread_isOn = false;
		if (graphicThread.joinable()) graphicThread.join();
	}
}

void Graphics::graphicMain()
{
	mediaHandle.setContextToMyThread();
	init();

	while (thread_isOn)
	{
		render();
	}
}

void Graphics::render()
{
	//transform stuff

	
	//shader->Bind();
	//texture->bind();
	//shader->Update(*transform, *camera);
	clear();
	//mesh->draw();
	mediaHandle.display();	
}

void Graphics::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::setClearParameters(float r, float g, float b, float a, float depth)
{
	glClearDepth(depth);
	glClearColor(r, g, b, a);
}

void Graphics::clear(float r, float g, float b, float a, float depth)
{
	setClearParameters(r, g, b, a, depth);
	clear();
}

