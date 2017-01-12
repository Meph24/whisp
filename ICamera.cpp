#include "ICamera.h"

#include <GL/glew.h>

ICamera::ICamera():
posX(0), posY(0), posZ(0)
{
	
}


ICamera::~ICamera()
{

}


void ICamera::applyAgain()
{
	glMultMatrixf(mat);
}

void ICamera::setPosition(float x, float y, float z)
{
	posX = x;//TODO update
	posY = y;
	posZ = z;
	changeAngleBy(0, 0, 0);
}
