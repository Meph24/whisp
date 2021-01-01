#include "ICamera3D.h"

#include "glmutils.hpp"

#include <iostream>

ICamera3D::ICamera3D( Eye& eye )
	: ICamera(eye)
	, ml()
{}

vec3 ICamera3D::getNormal(vec3 v)
{
	return glm::normalize(ml * v);
}

vec3 ICamera3D::getUpVector()
{
	return getNormal({0,1,0});
}

vec3 ICamera3D::getForwardVector()
{
	return getNormal({0,0,-1});
}

void ICamera3D::applyFrustum()
{
	glMatrixMode(GL_PROJECTION);
	float pixelOffsetX=(((int)width+1)%2);
	float pixelOffsetY=(((int)height+1)%2);
	leftPart=-((width+pixelOffsetX)/height)*zoom;
	rightPart=((width-pixelOffsetX)/height)*zoom;
	downPart=-((height+pixelOffsetY)/height)*zoom;
	upPart=((height-pixelOffsetY)/height)*zoom;
	glFrustum(leftPart*minView, rightPart*minView, downPart*minView, upPart*minView, minView, maxView);

	ml.loadIdentity();
	ml = ml 
		* glm::rotateDeg(-eye->rotation.y, vec3(0, 1, 0))
		* glm::rotateDeg(-eye->rotation.x, vec3(1, 0, 0))
		* glm::rotateDeg(-eye->rotation.z, vec3(0, 0, 1));
}

vec3 ICamera3D::getNearNormal()
{
	return getNormal(vec3(0,0,1));
}

vec3 ICamera3D::getFarNormal()
{
	return getNormal(vec3(0,0,-1));
}

vec3 ICamera3D::getRightNormal()
{
	return getNormal(vec3(1,0,rightPart));
}

vec3 ICamera3D::getLeftNormal()
{
	return getNormal(vec3(-1,0,-leftPart));
}

vec3 ICamera3D::getUpperNormal()
{
	return getNormal(vec3(0,1,upPart));
}

vec3 ICamera3D::getLowerNormal()
{
	return getNormal(vec3(0,-1,-downPart));
}
