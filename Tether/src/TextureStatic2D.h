#pragma once

#include "ITexture.h"

#include <string>

class TextureStatic2D: public ITexture
{
	const char * fName;
public:
	void bind();
	void update();
	void update(TexParamSet * texParams);
	TextureStatic2D(TexParamSet * texParams, const char * fileName);
	~TextureStatic2D();
};
