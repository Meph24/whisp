#pragma once
#include "Zombie_MouseInput.h"
class Zombie_KeyInput
{
	ICamera3D * cam;
	Zombie_MouseInput * mouseInput;
	float frontVec;
	float rightVec;



	void clampMovVec();
public:
	Zombie_KeyInput(Zombie_MouseInput * toToggle,ICamera3D * toMove);
	~Zombie_KeyInput();


	float speed=20;// reference speed


	void menuButton();
	void walkForward(bool enable);
	void walkBack(bool enable);
	void walkRight(bool enable);
	void walkLeft(bool enable);

	void applyMovement(float seconds);
};

