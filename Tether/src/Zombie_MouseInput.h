#pragma once
#include "ICamera3D.h"
#include "SFML/Window.hpp"
class Zombie_MouseInput
{
	ICamera3D * cam;
	sf::Window * w;
	bool active;
public:

	float sensitivityX;
	float sensitivityY;

	Zombie_MouseInput(ICamera3D * camera, sf::Window * window);
	~Zombie_MouseInput();
	void enable();
	void disable();
	void toggleEnable();
	void mouseMovedX(int pos);
	void mouseMovedY(int pos);

	ICamera3D * getCam();
};