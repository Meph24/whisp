#include "CameraFP.h"


CameraFP::CameraFP()
{

}

DivisionPlane CameraFP::getNearPlane()
{
	vec3 normal=getNearNormal();
	DivisionPlane ret(normal,minView+glm::dot(normal,vec3(posX,posY,posZ)));
	return ret;
}

DivisionPlane CameraFP::getFarPlane(float overrideViewDist)
{
	float dist=maxView;
	if(overrideViewDist!=-1) dist=overrideViewDist;
	vec3 normal=getFarNormal();
	DivisionPlane ret(normal,dist+glm::dot(normal,vec3(posX,posY,posZ)));
	return ret;
}

DivisionPlane CameraFP::getRightPlane()
{
	vec3 normal=getRightNormal();
	DivisionPlane ret(normal,glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

DivisionPlane CameraFP::getLeftPlane()
{
	vec3 normal=getLeftNormal();
	DivisionPlane ret(normal,glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

DivisionPlane CameraFP::getUpperPlane()
{
	vec3 normal=getUpperNormal();
	DivisionPlane ret(normal,glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

DivisionPlane CameraFP::getLowerPlane()
{
	vec3 normal=getLowerNormal();
	DivisionPlane ret(normal,glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

CameraFP::~CameraFP()
{

}

#include <iostream>
void CameraFP::apply()
{
	applyFrustum();
	glMatrixMode(GL_MODELVIEW);
	glRotatef(gamma, 0, 0, 1);
	glRotatef(alpha, 1, 0, 0);
	glRotatef(beta, 0, 1, 0);
	glTranslatef(-posX, -posY, -posZ);
}
