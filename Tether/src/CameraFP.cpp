#include "CameraFP.h"

#include <iostream>

CameraFP::CameraFP(Eye& eye)
	: ICamera3D(eye)
{}

DivisionPlane CameraFP::getNearPlane()
{
	vec3 normal=getNearNormal();
	DivisionPlane ret(normal,minView+glm::dot(normal, eye->offsetFromEntity()));
	return ret;
}

DivisionPlane CameraFP::getFarPlane(float overrideViewDist)
{
	float dist=maxView;
	if(overrideViewDist!=-1) dist=overrideViewDist;
	vec3 normal=getFarNormal();
	DivisionPlane ret(normal, dist + glm::dot(normal, eye->offsetFromEntity()));
	return ret;
}

DivisionPlane CameraFP::getRightPlane()
{
	vec3 normal=getRightNormal();
	DivisionPlane ret(normal,glm::dot(normal, eye->offsetFromEntity()));
	return ret;
}

DivisionPlane CameraFP::getLeftPlane()
{
	vec3 normal=getLeftNormal();
	DivisionPlane ret(normal,glm::dot(normal, eye->offsetFromEntity()));
	return ret;
}

DivisionPlane CameraFP::getUpperPlane()
{
	vec3 normal=getUpperNormal();
	DivisionPlane ret(normal,glm::dot(normal, eye->offsetFromEntity()));
	return ret;
}

DivisionPlane CameraFP::getLowerPlane()
{
	vec3 normal=getLowerNormal();
	DivisionPlane ret(normal,glm::dot(normal, eye->offsetFromEntity()));
	return ret;
}

void CameraFP::apply()
{
	applyFrustum();
	glMatrixMode(GL_MODELVIEW);
	glRotatef(eye->rotation.z, 0, 0, 1);
	glRotatef(eye->rotation.x, 1, 0, 0);
	glRotatef(eye->rotation.y, 0, 1, 0);
	const vec3& pos = eye->offsetFromEntity();
	glTranslatef(-pos.x, -pos.y, -pos.z);
}
