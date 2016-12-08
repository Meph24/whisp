#pragma once
#include "ICamera.h"
class CameraTP:
	public ICamera
{
	float zoom;
	float fov;
	float alpha, beta, gamma;
public:
	void reset();
	void resetAngle();
	void changeAngleBy(float alpha, float beta, float gamma);
	void changeZoomBy(float factor);
	void resetZoom();
	CameraTP();
	~CameraTP();
};

