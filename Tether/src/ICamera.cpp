#include "ICamera.h"

ICamera::ICamera( Eye& eye)
	: eye(&eye)
	, zoom(1)
	, width(600), height(600)
	, maxView(16*65536.0f), minView(1.0f)
{}

void ICamera::applyFresh()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	apply();
}
