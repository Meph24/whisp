#include "SFMLWindow.hpp"
#include <SFML/Window/WindowStyle.hpp>

bool SFMLWindow::isOpen() const { return w.isOpen(); }
void SFMLWindow::close() { w.close(); }
void SFMLWindow::display() { w.display(); }

float SFMLWindow::width() const { return w.getSize().x; }
float SFMLWindow::height() const { return w.getSize().y; }
void SFMLWindow::activate() { w.setActive(true); }

SFMLWindow::SFMLWindow(unsigned int width, unsigned int height, string title, uint32_t style, sf::ContextSettings contextsettings )
	: w(sf::VideoMode(width, height), title, style, contextsettings)
	, event_poll_counter(0) {}


/*
vector<InputEvent> SFMLWindow::mapSFEventToInputEvents( const sf::Event& sfe, const WallClock::time_point& poll_time)
{
	using namespace InputEventSpec;

	vector<InputEvent> ret;

	switch(sfe.type)
	{
		case sf::Event::EventType::Closed:
			ret.emplace_back(eventId(EventType::Window, 0), 1.0f, poll_time);
			break;
		case sf::Event::EventType::Resized:
			ret.emplace_back(eventId(EventType::Window, 1), sfe.size.width, poll_time);
			ret.emplace_back(eventId(EventType::Window, 2), sfe.size.height, poll_time);
			break;
		case sf::Event::EventType::LostFocus:
			ret.emplace_back(eventId(EventType::Window, 3), 0.0f, poll_time);
			break;
		case sf::Event::EventType::GainedFocus:
			ret.emplace_back(eventId(EventType::Window, 3), 1.0f, poll_time);
			break;
		case sf::Event::EventType::TextEntered:
			//TODO create elaborate text-listener objects
			//this is a future feature of this component
			break;
		case sf::Event::EventType::MouseEntered:
			ret.emplace_back(eventId(EventType::Window, 4), 1.0f, poll_time);
			break;
		case sf::Event::EventType::MouseLeft:
			ret.emplace_back(eventId(EventType::Window, 4), 0.0f, poll_time);
			break;
		case sf::Event::EventType::JoystickDisconnected:
			ret.emplace_back(eventId(EventType::Window, 5), 0.0f, poll_time);
			break;
		case sf::Event::EventType::JoystickConnected:
			ret.emplace_back(eventId(EventType::Window, 5), 1.0f, poll_time);
			break;
		case sf::Event::EventType::KeyPressed:
			ret.emplace_back(eventId(EventType::Keyboard, sfe.key.code), 1.0f, poll_time);
			break;
		case sf::Event::EventType::KeyReleased:
			ret.emplace_back(eventId(EventType::Keyboard, sfe.key.code), 0.0f, poll_time);
			break;
		case sf::Event::EventType::MouseMoved:
			ret.emplace_back(eventId(EventType::Mouse,mouse_movement_offset + 0), sfe.mouseMove.x, poll_time);
			ret.emplace_back(eventId(EventType::Mouse,mouse_movement_offset + 1), sfe.mouseMove.y, poll_time);
			break;
		case sf::Event::EventType::MouseWheelScrolled:
			//when handling this event we also get the mouse position at the time, so we handle this
			// as well
			ret.emplace_back(eventId(EventType::Mouse,mouse_movement_offset + 0), sfe.mouseWheelScroll.x, poll_time);
			ret.emplace_back(eventId(EventType::Mouse,mouse_movement_offset + 1), sfe.mouseWheelScroll.y, poll_time);

			ret.emplace_back(eventId(EventType::Mouse, mouse_wheel_offset + sfe.mouseWheelScroll.wheel), sfe.mouseWheelScroll.delta, poll_time);
			break;
		case sf::Event::EventType::MouseButtonPressed:
			//when handling this event we also get the mouse position at the time, so we handle this
			// as well
			ret.emplace_back(eventId(EventType::Mouse,mouse_movement_offset + 0), sfe.mouseButton.x, poll_time);
			ret.emplace_back(eventId(EventType::Mouse,mouse_movement_offset + 1), sfe.mouseButton.y, poll_time);

			ret.emplace_back(eventId(EventType::Mouse, mouse_button_offset + sfe.mouseButton.button), 1.0f, poll_time);
			break;
		case sf::Event::EventType::MouseButtonReleased:
			//when handling this event we also get the mouse position at the time, so we handle this
			// as well
			ret.emplace_back(eventId(EventType::Mouse, mouse_movement_offset + 0), sfe.mouseButton.x, poll_time);
			ret.emplace_back(eventId(EventType::Mouse, mouse_movement_offset + 1), sfe.mouseButton.y, poll_time);
			ret.emplace_back(eventId(EventType::Mouse, mouse_button_offset + sfe.mouseButton.button), 0.0f, poll_time);
			break;
		case sf::Event::EventType::JoystickMoved:
			ret.emplace_back(eventId(EventType::Joystick, sfe.joystickButton.joystickId * joystick_id_offset 
					 + joystick_movement_offset + sfe.joystickMove.axis), sfe.joystickMove.position, poll_time);
			break;
		case sf::Event::EventType::JoystickButtonPressed:
			ret.emplace_back(eventId(EventType::Joystick, sfe.joystickButton.joystickId * joystick_id_offset + joystick_button_offset + sfe.joystickButton.button), 1.0f, poll_time);
			break;
		case sf::Event::EventType::JoystickButtonReleased:
			ret.emplace_back(eventId(EventType::Joystick, sfe.joystickButton.joystickId * joystick_id_offset + joystick_button_offset + sfe.joystickButton.button), 0.0f, poll_time);
			break;
		default:
			break;
	}
return ret;
}
*/

#define MOUSE_WHEEL_OFFSET 2
#define MOUSE_BUTTON_OFFSET 4 
#define JOYSTICK_ID_OFFSET 48
#define JOYSTICK_BUTTON_OFFSET 8

void SFMLWindow::mapSFEventToEventHandlerEvents( const sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer)
{
	switch (e.type)
	{
	case sf::Event::EventType::Closed:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 0, 1));
		break;

	case sf::Event::EventType::Resized:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 1, e.size.width));
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 2, e.size.height));
		break;

	case sf::Event::EventType::LostFocus:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 3, 1));
		break;

	case sf::Event::EventType::GainedFocus:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 3, 0));
		break;

	case sf::Event::EventType::TextEntered:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 4, e.text.unicode));
		break;

	case sf::Event::EventType::MouseEntered:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 5, 1));
		break;

	case sf::Event::EventType::MouseLeft:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 5, 0));
		break;

	case sf::Event::EventType::JoystickDisconnected:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 1));
		break;

	case sf::Event::EventType::JoystickConnected:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 0));
		break;

	case sf::Event::EventType::KeyPressed:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Keyboard, e.key.code, 1));
		break;

	case sf::Event::EventType::KeyReleased:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Keyboard, e.key.code, 0));
		break;

	case sf::Event::EventType::MouseMoved:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Mouse, 0, e.mouseMove.x));
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Mouse, 1, e.mouseMove.y));
		break;

	case sf::Event::EventType::MouseWheelScrolled:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Mouse, MOUSE_WHEEL_OFFSET + e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta));
		break;

	case sf::Event::EventType::MouseButtonPressed:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 1));
		break;

	case sf::Event::EventType::MouseButtonReleased:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 0));
		break;

	case sf::Event::EventType::JoystickMoved:
		eventBuffer.write(EventHandler::createEvent(EventHandler::Joystick, e.joystickMove.joystickId*JOYSTICK_ID_OFFSET + e.joystickMove.axis, e.joystickMove.position));
		break;

	case sf::Event::EventType::JoystickButtonPressed:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId*JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 1));
		break;

	case sf::Event::EventType::JoystickButtonReleased:
		eventBuffer.write(EventHandler::createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId *JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 0));
		break;
	
	default :
        break;

	}
}
