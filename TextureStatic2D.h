#pragma once
#include "ITexture.h"
#include <string>
class TextureStatic2D :
	public ITexture
{
	const std::string& fName;
public:
	void bind();
	void update();
	void update(TexParamSet * texParams);
	TextureStatic2D(TexParamSet * texParams, const std::string& fileName);
	~TextureStatic2D();
};

