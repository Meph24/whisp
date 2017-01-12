#pragma once
#include "ICamera.h"
class CameraFP :
	public ICamera
{
	//float mat[16];
	//float posX, posY, posZ;
	
public:

	//void apply();
	//void setPosition(float x, float y, float z);

	void reset();
	void resetAngle();
	void changeAngleBy(float alpha, float beta, float gamma);
	void changeZoomBy(float factor);
	void resetZoom();
	CameraFP();
	~CameraFP();
};