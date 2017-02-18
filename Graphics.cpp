#include "Graphics.h"
#include <GL/glew.h>
#include <iostream>

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
}

void Graphics::init()
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
#include "clickibunti.h"
void Graphics::render()
{
	clear();
	clickibunti();
	mediaHandle.display();
}