#include "CameraFP.h"


CameraFP::CameraFP()
{

}


CameraFP::~CameraFP()
{

}

#include <iostream>
void CameraFP::apply()
{
	glMatrixMode(GL_PROJECTION);
	float pixelOffsetX=(((int)width+1)%2);
	float pixelOffsetY=(((int)height+1)%2);
	//std::cout<<width<<"	"<<height<<"	"<<pixelOffsetX<<"	"<<pixelOffsetY<<"	"<<std::endl;
	glFrustum(-((width+pixelOffsetX)/height)*zoom*minView, ((width-pixelOffsetX)/height)*zoom*minView, -((height+pixelOffsetY)/height)*zoom*minView, ((height-pixelOffsetY)/height)*zoom*minView, minView, maxView);
	glMatrixMode(GL_MODELVIEW);
	glRotatef(gamma, 0, 0, 1);
	glRotatef(alpha, 1, 0, 0);
	glRotatef(beta, 0, 1, 0);
	glTranslatef(-posX, -posY, -posZ);
}
