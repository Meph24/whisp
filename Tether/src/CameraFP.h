#pragma once
#include "ICamera3D.h"
class CameraFP :
	public ICamera3D
{
public:

	void apply();

	CameraFP();
	~CameraFP();
};