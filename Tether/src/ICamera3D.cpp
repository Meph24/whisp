#include "ICamera3D.h"


ICamera3D::ICamera3D():
ml(2),alpha(0), beta(0), gamma(0)
{
}

vec3 ICamera3D::getNormal(vec3 v)
{
	vec3 ret;
	ret.x = ml.curMatrix[0] * v.x + ml.curMatrix[4] * v.y + ml.curMatrix[8] * v.z;
	ret.y = ml.curMatrix[1] * v.x + ml.curMatrix[5] * v.y + ml.curMatrix[9] * v.z;
	ret.z = ml.curMatrix[2] * v.x + ml.curMatrix[6] * v.y + ml.curMatrix[10] * v.z;
	ret.normalize();
	return ret;
}

vec3 ICamera3D::getUpVector()
{
	return getNormal({0,1,0});
}

vec3 ICamera3D::getForwardVector()
{
	return getNormal({0,0,-1});
}

ICamera3D::~ICamera3D()
{
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
	ml.rotatef(-beta, 0, 1, 0);
	ml.rotatef(-alpha, 1, 0, 0);
	ml.rotatef(-gamma, 0, 0, 1);
}

vec3 ICamera3D::getNearNormal()
{
	return getNormal({0,0,1});
}

vec3 ICamera3D::getFarNormal()
{
	return getNormal({0,0,-1});
}

vec3 ICamera3D::getRightNormal()
{
	return getNormal({1,0,rightPart});
}

vec3 ICamera3D::getLeftNormal()
{
	return getNormal({-1,0,-leftPart});
}

vec3 ICamera3D::getUpperNormal()
{
	return getNormal({0,1,upPart});
}

vec3 ICamera3D::getLowerNormal()
{
	return getNormal({0,-1,-downPart});
}
