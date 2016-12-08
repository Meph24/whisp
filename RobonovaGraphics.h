#pragma once
#include "MathModel.h"
#include "ITexture.h"
class RobonovaGraphics
{
	ITexture * tex;
	void draw();
	void drawServo();
	void drawFoot(bool left, foot footAngleData);
	MathModel * model;
public:
	RobonovaGraphics(MathModel * myModel);
	~RobonovaGraphics();
};

