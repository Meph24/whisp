#pragma once
#include "ICamera3D.h"
class CameraTP: public ICamera3D
{
public:

	float dist;

	void apply();

	virtual DivisionPlane getNearPlane();
	virtual DivisionPlane getFarPlane();
	virtual DivisionPlane getRightPlane();
	virtual DivisionPlane getLeftPlane();
	virtual DivisionPlane getUpperPlane();
	virtual DivisionPlane getLowerPlane();

	CameraTP();
	~CameraTP();
};

