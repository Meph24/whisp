#include "SFMLHandle.h"

#include <GL\glew.h>
#include <iostream>


#define MOUSE_WHEEL_OFFSET 2
#define MOUSE_BUTTON_OFFSET 4 
#define JOYSTICK_ID_OFFSET 48
#define JOYSTICK_BUTTON_OFFSET 8




SFMLHandle::SFMLHandle()
{}

SFMLHandle::SFMLHandle(std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings) :
contextSettings(24, 8, 0, 3, 3),
window(sf::VideoMode(reswidth, resheight), name, sf::Style::None, contextSettings)
{
	//important because of the ability to activate the context in another thread
	window.setActive(false);
}


SFMLHandle::~SFMLHandle()
{
}

void SFMLHandle::createWindow(std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings)
{
	contextSettings.depthBits = settings.depth;
	contextSettings.stencilBits = settings.stencil;
	contextSettings.antialiasingLevel = settings.antialiasing;
	contextSettings.majorVersion = settings.openglmajor;
	contextSettings.minorVersion = settings.openglminor;

	
	window.create(sf::VideoMode(reswidth, resheight), name, sf::Style::None, contextSettings);

	//important because of the ability to activate the context in another thread
	window.setActive(false);
}

void SFMLHandle::mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer)
{
	switch (e.type)
	{
	case sf::Event::EventType::Closed:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 0, 1));
		break;

	case sf::Event::EventType::Resized:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 1, e.size.width));
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 2, e.size.height));
		break;

	case sf::Event::EventType::LostFocus:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 3, 1));
		break;

	case sf::Event::EventType::GainedFocus:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 3, 0));
		break;

	case sf::Event::EventType::TextEntered:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 4, e.text.unicode));
		break;

	case sf::Event::EventType::MouseEntered:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 5, 1));
		break;

	case sf::Event::EventType::MouseLeft:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 5, 0));
		break;

	case sf::Event::EventType::JoystickDisconnected:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 1));
		break;

	case sf::Event::EventType::JoystickConnected:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 0));
		break;

	case sf::Event::EventType::KeyPressed:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Keyboard, e.key.code, 1));
		break;

	case sf::Event::EventType::KeyReleased:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Keyboard, e.key.code, 0));
		break;

	case sf::Event::EventType::MouseMoved:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Mouse, 0, e.mouseMove.x));
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Mouse, 1, e.mouseMove.y));
		break;

	case sf::Event::EventType::MouseWheelScrolled:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Mouse, MOUSE_WHEEL_OFFSET + e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta));
		break;

	case sf::Event::EventType::MouseButtonPressed:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 1));
		break;

	case sf::Event::EventType::MouseButtonReleased:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 0));
		break;

	case sf::Event::EventType::JoystickMoved:
		eventBuffer.add(EH.createEvent(EventHandler::Joystick, e.joystickMove.joystickId*JOYSTICK_ID_OFFSET + e.joystickMove.axis, e.joystickMove.position));
		break;

	case sf::Event::EventType::JoystickButtonPressed:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId*JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 1));
		break;

	case sf::Event::EventType::JoystickButtonReleased:
		eventBuffer.add(EH.createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId *JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 0));
		break;
	}
}

void SFMLHandle::pollEvents()
{
	sf::Event e;
	Buffer < EventHandler::event, 4 >  eventBuffer;
	//handle events
	while (window.pollEvent(e))
	{
		preHandleEvent(e);
		
		mapSFEventToEventHandlerEvent(e, eventBuffer);
		//handle mapped events
		while (!eventBuffer.nodata())
		{
			EH.handle(eventBuffer.readnext());
		}
		eventBuffer.empty();

		postHandleEvent(e);
	}
}

void SFMLHandle::preHandleEvent(sf::Event& e)
{

	switch (e.type)
	{
		/*
		case sf::Event::EventType::MouseMoved:
		//hardcursorhandle.cursorMoved(e);
		break;

		case sf::Event::EventType::Resized:
		//hardcursorhandle.updateLockPosition();
		std::cout << SFMLHandle.window.getSize().x << "/" << SFMLHandle.window.getSize().y;
		break;

		case sf::Event::EventType::GainedFocus:
		//hardcursorhandle.updateLockPosition();	// no way to check if window was moved
		std::cout<< "Stuff !!";
		break;

		// _test_begin
		case sf::Event::EventType::KeyPressed:
		if (e.key.code == sf::Keyboard::Key::L) hardcursorhandle.setLocked(true);
		if (e.key.code == sf::Keyboard::Key::U) hardcursorhandle.setLocked(false);
		if (e.key.code == sf::Keyboard::Key::R) SFMLHandle.window.setSize (sf::Vector2u(400, 400));
		break;

		*/
		// _test_end

	}


}

void SFMLHandle::postHandleEvent(sf::Event& e)
{
	switch (e.type)
	{
		
		case sf::Event::EventType::Closed:
		window.close();

		break;


		// for testing

		case sf::Event::EventType::MouseButtonPressed:
		break;
		
	}
}

void SFMLHandle::setContextToMyThread()
{
	window.setActive(true);
}

void SFMLHandle::display()
{
	window.display();
}
