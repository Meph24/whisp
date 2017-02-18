#pragma once
#include "ICamera.h"
class ICamera3D :
	public ICamera
{

public:

	float alpha, beta, gamma;

	virtual void apply()=0;

	ICamera3D();
	~ICamera3D();
};