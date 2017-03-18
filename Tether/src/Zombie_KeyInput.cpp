#include "Zombie_KeyInput.h"


Zombie_KeyInput::Zombie_KeyInput(Zombie_MouseInput * toToggle, ICamera3D * toMove):
cam(toMove), mouseInput(toToggle), frontVec(0), rightVec(0)
{

}


Zombie_KeyInput::~Zombie_KeyInput()
{
	 
}

void Zombie_KeyInput::clampMovVec()
{
	if (frontVec > 1) frontVec = 1;
	if (frontVec < -1) frontVec = -1;
	if (rightVec > 1) rightVec = 1;
	if (rightVec < -1) rightVec = -1;
}

void Zombie_KeyInput::menuButton()
{
	mouseInput->toggleEnable();
}


void Zombie_KeyInput::walkForward(bool enable)
{
	if (enable)
		frontVec++;
	else
		frontVec--;

	clampMovVec();//just safety

}
void Zombie_KeyInput::walkBack(bool enable)
{
	if (enable)
		frontVec--;
	else
		frontVec++;

	clampMovVec();//just safety
}
void Zombie_KeyInput::walkRight(bool enable)
{
	if (enable)
		rightVec++;
	else
		rightVec--;

	clampMovVec();//just safety
}
void Zombie_KeyInput::walkLeft(bool enable)
{
	if (enable)
		rightVec--;
	else
		rightVec++;

	clampMovVec();//just safety
}

#include "MatrixLib2.h"
void Zombie_KeyInput::applyMovement(float seconds)
{
	float m11 = cos((-cam->beta) / 360 * TAU);
	float m12 = -sin((-cam->beta) / 360 * TAU);
	float m21 = -m12;//sin
	float m22 = m11;//cos

	float movZ = -m11*frontVec+m12*rightVec;
	float movX = -m21*frontVec+m22*rightVec;

	cam->posX += movX*speed*seconds;
	cam->posZ += movZ*speed*seconds;
}

