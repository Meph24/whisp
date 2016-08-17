#include "Texture.h"

#include "stb_image.h"
#include <cassert>
#include <exception>

#include <iostream>

Texture::Texture(const std::string& fileName)
{
	int width, height, numComponents;
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
		throw std::runtime_error("Texture " + fileName + " unable to load !");

	glGenTextures(1, &texture);							//1st Para: number of TextureIds generated ; 2nd Para array to store IDs
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}