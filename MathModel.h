#pragma once
#include "MatrixLib2.h"
typedef struct
{
	float a0;

	float a1;
	float a2;
	float a3;

	float a4;
} foot;

typedef struct
{
	float x;
	float y;
	float z;
} pos;

typedef struct
{
	float a0;

	float a1;
	float a2;
} arm;

class MathModel
{
public:

	MatrixLib2 ml;

	foot left;
	foot right;
	arm leftArm;
	arm rightArm;

	float footToBody[16];
	float bodyToLeftArm[16];
	float bodyToRightArm[16];


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

	void setLeftFoot(foot f);
	void setRightFoot(foot f);
	void setLeftArm(arm a);
	void setRightArm(arm a);
	void setStandingFoot(bool b);
	pos getFootToFootPos();

	MathModel();
	~MathModel();
};

