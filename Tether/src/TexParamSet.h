#pragma once

#include <vector>
#include <GL/glew.h>

class TexParamSet
{
	std::vector<GLenum> enumI;
	std::vector<GLenum> enumF;
	std::vector<GLint> paramI;
	std::vector<GLfloat> paramF;

public:
	void addI(GLenum e, GLint i);
	void addF(GLenum e, GLfloat f);
	void apply();
	TexParamSet();
	~TexParamSet();
};

