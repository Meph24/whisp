
#pragma once

#include <string>

class IMediaHandle
{
public:
	struct ContextSettings
	{
		int depth, stencil, antialiasing, openglmajor, openglminor;
	};

public:

	virtual void createWindow(std::string name, int reswidth, int resheigt, ContextSettings& contextSettings) = 0;
	virtual void pollEvents() = 0;
};