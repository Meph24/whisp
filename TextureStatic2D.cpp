#include "TextureStatic2D.h"

#include "stb_image.h"
#include <iostream>
TextureStatic2D::TextureStatic2D(TexParamSet * texParams, const char * fileName)
:ITexture(texParams),
fName(fileName)
{
}


TextureStatic2D::~TextureStatic2D()
{

}

void TextureStatic2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	/*for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
	{
		std::cout << "error code: " << err << std::endl;
	}*/
}

void TextureStatic2D::update()
{
	int width=0, height=0, numComponents;
	
	unsigned char * data = stbi_load(fName, &width, &height, &numComponents, 4);//"./res/font.png"

	if (data == NULL)
	{
		std::cout << "Texture " + std::string(fName) + " unable to load !";//throw std::runtime_error("Texture " + std::string(fName) + " unable to load !");
		return;
	}

	glGenTextures(1, &texID);		//1st Para: number of TextureIds generated ; 2nd Para array to store IDs
	bind();
	std::cout << texID << std::endl;
	params->apply();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void TextureStatic2D::update(TexParamSet * texParams)
{
	params = texParams;
	update();
}
