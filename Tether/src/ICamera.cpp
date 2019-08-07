#include "ICamera.h"


ICamera::ICamera():
posX(0), posY(0), posZ(0), zoom(1), width(600), height(600), maxView(16*65536.0f),minView(1.0f)//TODO
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
