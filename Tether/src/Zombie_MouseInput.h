#pragma once

class EntityPlayer;
class ICamera3D;

#include "SFML/Window.hpp"

class Zombie_MouseInput
{
	EntityPlayer * player;
	sf::Window * w;
	bool active;
public:

	float sensitivityX;
	float sensitivityY;
	float defaultSensitivityX;
	float defaultSensitivityY;

	Zombie_MouseInput(EntityPlayer * playerToSteer, sf::Window * window,float sensX,float sensY);
	~Zombie_MouseInput();
	void enable();
	void disable();
	void toggleEnable();
	void mouseMovedX(int pos);
	void mouseMovedY(int pos);
	void setSensitivityMultiplier(float multiplier=1);

	ICamera3D * getCam();
};
