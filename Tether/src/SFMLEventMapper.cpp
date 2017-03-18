#include "SFMLEventMapper.h"


SFMLEventMapper::SFMLEventMapper(EventHandler& EH)
	:
	EH(EH)
{
}

void SFMLEventMapper::handleEvent(sf::Event& e)
{

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

}