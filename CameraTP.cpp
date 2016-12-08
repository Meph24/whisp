#include "CameraTP.h"


CameraTP::CameraTP()
{
	reset();
}


CameraTP::~CameraTP()
{

}



void CameraTP::reset()
{
	resetAngle();
	resetZoom();
}

void CameraTP::resetAngle()
{
	alpha = 0;
	beta = 0;
	gamma = 0;
}

void CameraTP::changeAngleBy(float alpha, float beta, float gamma)
{

}

void CameraTP::changeZoomBy(float factor)
{
	zoom *= factor;
}

void CameraTP::resetZoom()
{

}