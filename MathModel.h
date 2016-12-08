#pragma once
#include "MatrixLib.h"
typedef struct
{
	float a0;

	float a1;
	float a2;
	float a3;

	float a4;
} foot;
class MathModel
{
public:

	MatrixLib ml;
	foot left;
	foot right;

	int standingFoot = 0;//0=more weight on left foot; 1=more weight on right foot

	float leftAlpha;//body to foot
	float leftBeta;
	float rightAlpha;
	float rightBeta;

	//define	x		as front
	//			y		as up
	//			z		as right
	//			alpha	as to front
	//			beta	as to side (outwards)

	float relX;//relative position from standing foot to other foot
	float relY;
	float relZ;


	void update();
	void draw();

	MathModel();
	~MathModel();
};

