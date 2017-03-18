#pragma once

#include <SFML/Window.hpp>
#include "EventHandler.h"


#define MOUSE_WHEEL_OFFSET 2
#define MOUSE_BUTTON_OFFSET 4 
#define JOYSTICK_ID_OFFSET 48
#define JOYSTICK_BUTTON_OFFSET 8


class SFMLEventMapper
{
	EventHandler& EH;

public:
	SFMLEventMapper(EventHandler& EH);

	void handleEvent(sf::Event& e);
};
