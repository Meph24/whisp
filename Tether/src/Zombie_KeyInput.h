#pragma once
class Zombie_MouseInput;
class ICamera3D;
#include "vec3.h"
class Zombie_KeyInput
{
	ICamera3D * cam;

	Zombie_MouseInput * mouseInput;
	float frontVec;
	float rightVec;



	void clampMovVec();
public:
	Zombie_KeyInput(Zombie_MouseInput * toToggle,ICamera3D * referenceCam);
	~Zombie_KeyInput();



	void menuButton();
	void walkForward(bool enable);
	void walkBack(bool enable);
	void walkRight(bool enable);
	void walkLeft(bool enable);

	vec3 getVelocity();
};

#include "Zombie_MouseInput.h"
#include "ICamera3D.h"
