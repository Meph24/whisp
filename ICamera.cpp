#include "ICamera.h"

#include <GL/glew.h>

ICamera::ICamera():
posX(0), posY(0), posZ(0)
{
	
}


ICamera::~ICamera()
{

}


void ICamera::apply()
{
	glMultMatrixf(mat);
}

void ICamera::setPosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
	changeAngleBy(0, 0, 0);
}
