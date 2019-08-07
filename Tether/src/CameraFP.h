#pragma once
#include "ICamera3D.h"
class CameraFP: public ICamera3D
{
public:

	void apply();
	virtual DivisionPlane getNearPlane();
	virtual DivisionPlane getFarPlane(float overrideViewDist=-1);
	virtual DivisionPlane getRightPlane();
	virtual DivisionPlane getLeftPlane();
	virtual DivisionPlane getUpperPlane();
	virtual DivisionPlane getLowerPlane();

	CameraFP();
	~CameraFP();
};
