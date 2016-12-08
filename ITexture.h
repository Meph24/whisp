#pragma once

#include "TexParamSet.h"
#include <GL\glew.h>
class ITexture
{
protected:
	GLuint texID;
	TexParamSet * params;
public:
	virtual void update() = 0;
	virtual void update(TexParamSet * texParams) = 0;
	virtual void bind() = 0;
	ITexture(TexParamSet * texParams);
	~ITexture();
};

