#pragma once
#include "MathModel.h"
#include "ITexture.h"
class RobonovaGraphics
{
public:
	ITexture * tex;
	void draw();
	void drawServo();
	void drawFoot(bool left, foot footAngleData);
	void drawArm(bool left, arm footAngleData);
	MathModel * model;
	RobonovaGraphics(MathModel * myModel);
	~RobonovaGraphics();
};

