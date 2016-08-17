#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL\glew.h>

class Texture
{
	GLuint texture;

public:
	Texture(const std::string& filename);
	~Texture();

	void bind();
};

#endif //TEXTURE_H

