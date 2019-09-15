#include "CameraTP.h"


CameraTP::CameraTP(): dist(4)
{

}


DivisionPlane CameraTP::getNearPlane()
{
	vec3 normal=getNearNormal();
	DivisionPlane ret(
						normal,
						minView - dist + 
						glm::dot(
								normal,
								vec3(posX,posY,posZ)
								)
					 );
	return ret;
}

DivisionPlane CameraTP::getFarPlane(float overrideViewDist)
{
	float dist=maxView;
	if(overrideViewDist!=-1) dist=overrideViewDist;
	vec3 normal=getFarNormal();
	DivisionPlane ret(normal,dist+glm::dot(normal,vec3(posX,posY,posZ)));
	return ret;
}

DivisionPlane CameraTP::getRightPlane()
{
	vec3 normal=getRightNormal();
	DivisionPlane ret(normal,minView-dist+glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

DivisionPlane CameraTP::getLeftPlane()
{
	vec3 normal=getLeftNormal();
	DivisionPlane ret(normal,minView-dist+glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

DivisionPlane CameraTP::getUpperPlane()
{
	vec3 normal=getUpperNormal();
	DivisionPlane ret(normal,minView-dist+glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

DivisionPlane CameraTP::getLowerPlane()
{
	vec3 normal=getLowerNormal();
	DivisionPlane ret(normal,minView-dist+glm::dot(normal,vec3(posX,posY,posZ)));//TODO
	return ret;
}

CameraTP::~CameraTP()
{

}



void CameraTP::apply()
{
	applyFrustum();
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0, 0, -dist);
	glRotatef(gamma, 0, 0, 1);
	glRotatef(alpha, 1, 0, 0);
	glRotatef(beta, 0, 1, 0);
	glTranslatef(-posX, -posY, -posZ);
}
