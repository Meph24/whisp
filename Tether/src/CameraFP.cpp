#include "CameraFP.h"


CameraFP::CameraFP()
{

}


CameraFP::~CameraFP()
{

}


void CameraFP::apply()
{
	glMatrixMode(GL_PROJECTION);
	glFrustum(-aspect*zoom, aspect*zoom, -zoom, zoom, minView, maxView);
	glMatrixMode(GL_MODELVIEW);
	glRotatef(gamma, 0, 0, 1);
	glRotatef(alpha, 1, 0, 0);
	glRotatef(beta, 0, 1, 0);
	glTranslatef(-posX, -posY, -posZ);
}
