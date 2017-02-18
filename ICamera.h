#pragma once

#include <GL/glew.h>
class ICamera
{
public:

	float posX, posY, posZ;
	float zoom;// = tan(fov/2)
	float aspect;// width/height of screen

	float maxView, minView;//min and max view distance

	//calculates the matrices and applies them to modelview and projection matrices
	virtual void apply()=0;
	void applyFresh();//resets the matrices and calls apply() afterwards


	ICamera();
	~ICamera();
};