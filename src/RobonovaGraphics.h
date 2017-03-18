#pragma once
#include "MathModel.h"
#include "ITexture.h"
#include "ICamera3D.h"
#include "PerformanceMeter.h"
class RobonovaGraphics
{
	PerformanceMeter * pm;
public:
	bool rotEn = 1;
	ITexture * tex;
	ICamera3D * cam;
	void draw();
	void drawServo();
	void drawFoot(bool left, foot footAngleData);
	void drawArm(bool right, arm footAngleData);
	MathModel * model;
	RobonovaGraphics(MathModel * myModel);
	~RobonovaGraphics();
};

