#include "Zombie_MouseInput.h"


Zombie_MouseInput::Zombie_MouseInput(ICamera3D * camera, sf::Window * window) :
cam(camera),
w(window),
sensitivityX(0.1f),
sensitivityY(0.1f)
{

}


Zombie_MouseInput::~Zombie_MouseInput()
{

}

void Zombie_MouseInput::enable()
{
	active = true;
	w->setMouseCursorVisible(false);
}

void Zombie_MouseInput::disable()
{
	active = false;
	w->setMouseCursorVisible(true);
}

void Zombie_MouseInput::toggleEnable()
{
	if (active) disable();
	else enable();
}

void Zombie_MouseInput::mouseMovedX(int pos)
{
	if (!active) return;
	sf::Vector2u v = w->getSize();
	sf::Vector2i win = w->getPosition();
	int xMid = v.x / 2;
	int yMid = v.y / 2;
	int xDif = pos - xMid;
	cam->beta += xDif*sensitivityX;
	sf::Mouse::setPosition({ xMid + win.x, yMid + win.y });
}
void Zombie_MouseInput::mouseMovedY(int pos)
{
	if (!active) return;
	sf::Vector2u v = w->getSize();
	sf::Vector2i win = w->getPosition();
	int xMid = v.x / 2;
	int yMid = v.y / 2;
	int yDif = pos - yMid;
	cam->alpha += yDif*sensitivityY;
	sf::Mouse::setPosition({ xMid + win.x, yMid + win.y });
}

ICamera3D * Zombie_MouseInput::getCam()
{
	return cam;
}