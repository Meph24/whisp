#include "ICamera.h"


ICamera::ICamera():
posX(0), posY(0), posZ(0), zoom(1), minView(1.0f), maxView(8*65536.0f),width(600), height(600)
{
	
}


ICamera::~ICamera()
{

}


void ICamera::applyFresh()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	apply();
}
