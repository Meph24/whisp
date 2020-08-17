
#include "Zombie_MouseInput.h"
#include "EntityPlayer.h"
#include "CameraTP.h"

Zombie_MouseInput::Zombie_MouseInput(EntityPlayer * playerToSteer, sf::Window * window,float sensX,float sensY) :
player(playerToSteer),
w(window),
sensitivityX(sensX),
sensitivityY(sensY),
defaultSensitivityX(sensX),
defaultSensitivityY(sensY)
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
	player->cam->beta += xDif*sensitivityX;
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
	player->cam->alpha += yDif*sensitivityY;
	sf::Mouse::setPosition({ xMid + win.x, yMid + win.y });
}

ICamera3D * Zombie_MouseInput::getCam()
{
	return player->cam;
}

void Zombie_MouseInput::setSensitivityMultiplier(float multiplier)
{
	sensitivityX=defaultSensitivityX*multiplier;
	sensitivityY=defaultSensitivityY*multiplier;
}
