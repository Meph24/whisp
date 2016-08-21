#include "OSHandle.h"

#include <GL\glew.h>
#include <iostream>

#define MOUSE_WHEEL_OFFSET 2
#define MOUSE_BUTTON_OFFSET 4 
#define JOYSTICK_ID_OFFSET 48
#define JOYSTICK_BUTTON_OFFSET 8


OSHandle::OSHandle() :
contextSettings(24, 8, 0, 3, 3),
window(sf::VideoMode(800, 600), "This is a test !", sf::Style::None, contextSettings)

{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

}


OSHandle::~OSHandle()
{
}

void OSHandle::pollEvents()
{
	sf::Event e;
	//handle events
	while (window.pollEvent(e))
	{
		preHandleEvent(e);
		switch (e.type)
		{
		case sf::Event::EventType::Closed:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 0, 1));
			break;

		case sf::Event::EventType::Resized:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 1, e.size.width));
			EH.handle(EH.createEvent(EventHandler::eventType::System, 2, e.size.height));
			break;

		case sf::Event::EventType::LostFocus:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 3, 1));
			break;

		case sf::Event::EventType::GainedFocus:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 3, 0));
			break;

		case sf::Event::EventType::TextEntered:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 4, e.text.unicode));
			break;

		case sf::Event::EventType::MouseEntered:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 5, 1));
			break;

		case sf::Event::EventType::MouseLeft:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 5, 0));
			break;

		case sf::Event::EventType::JoystickDisconnected:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 1));
			break;

		case sf::Event::EventType::JoystickConnected:
			EH.handle(EH.createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 0));
			break;

		case sf::Event::EventType::KeyPressed:
			EH.handle(EH.createEvent(EventHandler::eventType::Keyboard, e.key.code, 1));
			break;

		case sf::Event::EventType::KeyReleased:
			EH.handle(EH.createEvent(EventHandler::eventType::Keyboard, e.key.code, 0));
			break;

		case sf::Event::EventType::MouseMoved:
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 0, e.mouseMove.x));
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 1, e.mouseMove.y));
			break;

		case sf::Event::EventType::MouseWheelScrolled:
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 0, e.mouseWheelScroll.x));
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 1, e.mouseWheelScroll.y));
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, MOUSE_WHEEL_OFFSET + e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta));
			break;

		case sf::Event::EventType::MouseButtonPressed:
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 0, e.mouseButton.x));
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 1, e.mouseButton.y));
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 1));
			break;

		case sf::Event::EventType::MouseButtonReleased:
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 0, e.mouseButton.x));
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, 1, e.mouseButton.y));
			EH.handle(EH.createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 0));
			break;

		case sf::Event::EventType::JoystickMoved:
			EH.handle(EH.createEvent(EventHandler::Joystick, e.joystickMove.joystickId*JOYSTICK_ID_OFFSET + e.joystickMove.axis, e.joystickMove.position));
			break;

		case sf::Event::EventType::JoystickButtonPressed:
			EH.handle(EH.createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId*JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 1));
			break;

		case sf::Event::EventType::JoystickButtonReleased:
			EH.handle(EH.createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId *JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 0));
			break;
		}

		postHandleEvent(e);
	}
}

void OSHandle::preHandleEvent(sf::Event& e)
{

	switch (e.type)
	{
		/*
		case sf::Event::EventType::MouseMoved:
		//hardcursorhandle.cursorMoved(e);
		break;

		case sf::Event::EventType::Resized:
		//hardcursorhandle.updateLockPosition();
		std::cout << oshandle.window.getSize().x << "/" << oshandle.window.getSize().y;
		break;

		case sf::Event::EventType::GainedFocus:
		//hardcursorhandle.updateLockPosition();	// no way to check if window was moved
		std::cout<< "Stuff !!";
		break;

		// _test_begin
		case sf::Event::EventType::KeyPressed:
		if (e.key.code == sf::Keyboard::Key::L) hardcursorhandle.setLocked(true);
		if (e.key.code == sf::Keyboard::Key::U) hardcursorhandle.setLocked(false);
		if (e.key.code == sf::Keyboard::Key::R) oshandle.window.setSize (sf::Vector2u(400, 400));
		break;

		*/
		// _test_end

	}


}

void OSHandle::postHandleEvent(sf::Event& e)
{
	switch (e.type)
	{
		/*
		case sf::Event::EventType::Closed:
		window.close();

		break;


		// for testing

		case sf::Event::EventType::MouseButtonPressed:
		break;
		*/
	}
}
