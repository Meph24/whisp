#pragma once
#include "ICamera3D.h"
class CameraTP:
	public ICamera3D
{
public:

	float dist;

	void apply();

	CameraTP();
	~CameraTP();
};

