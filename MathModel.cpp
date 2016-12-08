#include "MathModel.h"


MathModel::MathModel() :
ml(2)
{
	left.a0 = 0;
	left.a1 = 0;
	left.a2 = 60;
	left.a3 = 0;
	left.a4 = 0;
	right.a0 = left.a0;
	right.a1 = left.a1;
	right.a2 = left.a2;
	right.a3 = left.a3;
	right.a4 = left.a4;
}


MathModel::~MathModel()
{

}

void MathModel::update()
{
	leftAlpha  =  left.a1 +  left.a2 +  left.a3 - 60;
	rightAlpha = right.a1 + right.a2 + right.a3 - 60;
	leftBeta  =  left.a0 +  left.a4;
	rightBeta = right.a0 + right.a4;

	ml.loadIdentity();

	if (standingFoot == 0)
	{

		ml.translatef(0.0f, 11.75f, 0);//left leg->body
		ml.rotatef(left.a4, 1.0f, 0.0f, 0.0f);
		ml.translatef(0.0f, 40.5f, 0);

		ml.rotatef(-left.a3, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, 47.75f, 0);

		ml.rotatef(-left.a2, 0.0f, 0.0f, 1.0f);
		ml.rotatef(32, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, 57.0f, 0);
		ml.rotatef(28, 0.0f, 0.0f, 1.0f);

		ml.rotatef(-left.a1, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, 40.63f, 0);

		ml.rotatef(left.a0, 1.0f, 0.0f, 0.0f);
		ml.translatef(0.0f, 0.0f, 28.25f);

		//body
		
		ml.translatef(0.0f, 0.0f, 28.25f);//body->right leg
		ml.rotatef(right.a0, 1.0f, 0.0f, 0.0f);

		ml.translatef(0.0f, -40.63f, 0);
		ml.rotatef(right.a1, 0.0f, 0.0f, 1.0f);

		ml.rotatef(-28, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, -57.0f, 0);
		ml.rotatef(-32, 0.0f, 0.0f, 1.0f);
		ml.rotatef(right.a2, 0.0f, 0.0f, 1.0f);

		ml.translatef(0.0f, -47.75f, 0);
		ml.rotatef(right.a3, 0.0f, 0.0f, 1.0f);

		ml.translatef(0.0f, -40.5f, 0);
		ml.rotatef(right.a4, 1.0f, 0.0f, 0.0f);
		ml.translatef(0.0f, -11.75f, 0);
	}

	else
	{
		ml.translatef(0.0f, 11.75f, 0);//right leg->body
		ml.rotatef(-right.a4, 1.0f, 0.0f, 0.0f);
		ml.translatef(0.0f, 40.5f, 0);

		ml.rotatef(-right.a3, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, 47.75f, 0);

		ml.rotatef(-right.a2, 0.0f, 0.0f, 1.0f);
		ml.rotatef(32, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, 57.0f, 0);
		ml.rotatef(28, 0.0f, 0.0f, 1.0f);

		ml.rotatef(-right.a1, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, 40.63f, 0);

		ml.rotatef(-right.a0, 1.0f, 0.0f, 0.0f);
		ml.translatef(0.0f, 0.0f, -28.25f);

		//body

		ml.translatef(0.0f, 0.0f, -28.25f);//body->left leg
		ml.rotatef(-left.a0, 1.0f, 0.0f, 0.0f);

		ml.translatef(0.0f, -40.63f, 0);
		ml.rotatef(left.a1, 0.0f, 0.0f, 1.0f);

		ml.rotatef(-28, 0.0f, 0.0f, 1.0f);
		ml.translatef(0.0f, -57.0f, 0);
		ml.rotatef(-32, 0.0f, 0.0f, 1.0f);
		ml.rotatef(left.a2, 0.0f, 0.0f, 1.0f);

		ml.translatef(0.0f, -47.75f, 0);
		ml.rotatef(left.a3, 0.0f, 0.0f, 1.0f);

		ml.translatef(0.0f, -40.5f, 0);
		ml.rotatef(-left.a4, 1.0f, 0.0f, 0.0f);
		ml.translatef(0.0f, -11.75f, 0);
	}

	relX = ml.curMatrix[12];
	relY = ml.curMatrix[13];
	relZ = ml.curMatrix[14];
}

