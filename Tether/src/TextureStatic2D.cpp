#include "TextureStatic2D.h"

//#include "stb_image.h"
#include <iostream>
#include <SFML/Graphics.hpp>
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
	//TODO check sfml image load capabilities
	int width=0, height=0;
	//int numComponents;
	sf::Image img;
	if (!img.loadFromFile(fName))
	{
		std::cout << "Texture " + std::string(fName) + " unable to load !";//throw std::runtime_error("Texture " + std::string(fName) + " unable to load !");
		return;
	}
	auto a=img.getSize();
	width=a.x;
	height=a.y;

	unsigned char * data = (unsigned char *)img.getPixelsPtr();//stbi_load(fName, &width, &height, &numComponents, 4);//"./res/font.png"


	glGenTextures(1, &texID);		//1st Para: number of TextureIds generated ; 2nd Para array to store IDs
	bind();
	std::cout << "Texture " << fName<<" loaded with texID" << texID << std::endl;
	params->apply();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//stbi_image_free(data);
}

void TextureStatic2D::update(TexParamSet * texParams)
{
	params = texParams;
	update();
}
