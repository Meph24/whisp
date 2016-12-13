#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader
{
	enum
	{
		TRANSFORM_U,

		NUM_UNIFORM
	};

	static const unsigned int NUM_SHADER = 2;
	GLuint program;
	GLuint shaders[NUM_SHADER];
	GLuint uniforms[NUM_UNIFORM];

public:
	Shader(const std::string& filename);
	~Shader();
	void Bind();
	void Update(const Transform& transform, const Camera& camera);
};
#endif