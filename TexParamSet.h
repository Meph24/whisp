#pragma once
#include <GL\glew.h>
class TexParamSet
{
	GLenum * enumI;
	GLenum * enumF;
	GLint * paramI;
	GLfloat * paramF;
	int iCount;
	int fCount;
	int iMax;
	int fMax;

public:
	//return: true if success
	bool addI(GLenum e, GLint i);
	//return: true if success
	bool addF(GLenum e, GLfloat f);
	void apply();
	TexParamSet(int maxParamsI, int maxParamsF);
	~TexParamSet();
};

