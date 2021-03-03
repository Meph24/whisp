#include "SFMLWindow.hpp"
#include <SFML/Window/WindowStyle.hpp>

#include <utility>

using std::pair;

bool SFMLWindow::isOpen() const { return w.isOpen(); }
void SFMLWindow::close() { w.close(); }
void SFMLWindow::display() { w.display(); }

float SFMLWindow::width() const { return w.getSize().x; }
float SFMLWindow::height() const { return w.getSize().y; }
void SFMLWindow::setActive(bool b) { w.setActive(b); }

SFMLWindow::SFMLWindow(	unsigned int width, unsigned int height, 
						string title, 
						uint32_t style, 
						sf::ContextSettings contextsettings)
	: w(sf::VideoMode(width, height), title, style, contextsettings)
{}

#define MOUSE_WHEEL_OFFSET 2
#define MOUSE_BUTTON_OFFSET 4 
#define JOYSTICK_ID_OFFSET 48
#define JOYSTICK_BUTTON_OFFSET 8

void SFMLWindow::mapSFEventToEventHandlerEvents( const sf::Event& e, deque<InputEvent>& event_backlog)
{
	switch (e.type)
	{
	case sf::Event::EventType::Closed:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 0, 1));
		break;

	case sf::Event::EventType::Resized:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 1, e.size.width));
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 2, e.size.height));
		break;

	case sf::Event::EventType::LostFocus:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 3, 1));
		break;

	case sf::Event::EventType::GainedFocus:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 3, 0));
		break;

	case sf::Event::EventType::TextEntered:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 4, e.text.unicode));
		break;

	case sf::Event::EventType::MouseEntered:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 5, 1));
		break;

	case sf::Event::EventType::MouseLeft:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 5, 0));
		break;

	case sf::Event::EventType::JoystickDisconnected:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 6 + e.joystickConnect.joystickId, 1));
		break;

	case sf::Event::EventType::JoystickConnected:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::System, 6 + e.joystickConnect.joystickId, 0));
		break;

	case sf::Event::EventType::KeyPressed:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Keyboard, e.key.code, 1));
		break;

	case sf::Event::EventType::KeyReleased:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Keyboard, e.key.code, 0));
		break;

	case sf::Event::EventType::MouseMoved:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Mouse, 0, e.mouseMove.x));
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Mouse, 1, e.mouseMove.y));
		break;

	case sf::Event::EventType::MouseWheelScrolled:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Mouse, MOUSE_WHEEL_OFFSET + e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta));
		break;

	case sf::Event::EventType::MouseButtonPressed:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 1));
		break;

	case sf::Event::EventType::MouseButtonReleased:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 0));
		break;

	case sf::Event::EventType::JoystickMoved:
		event_backlog.emplace_back(InputEvent(InputEvent::Joystick, e.joystickMove.joystickId*JOYSTICK_ID_OFFSET + e.joystickMove.axis, e.joystickMove.position));
		break;

	case sf::Event::EventType::JoystickButtonPressed:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Joystick, e.joystickButton.joystickId*JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 1));
		break;

	case sf::Event::EventType::JoystickButtonReleased:
		event_backlog.emplace_back(InputEvent(InputEvent::Type::Joystick, e.joystickButton.joystickId *JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 0));
		break;
	
	default :
        break;
	}
}

bool SFMLWindow::pollEvent(InputEvent& e)
{
	if(!isOpen()) return false;
	while(1)
	{
		if(event_backlog.empty())
		{
			sf::Event sfe;
			if(!w.pollEvent(sfe)) return false;

			//special event cases
			switch (sfe.type)
			{
				case sf::Event::EventType::Closed:
					close();
					return false;
				break;

				case sf::Event::MouseMoved:
					if(mouse_locked) 
					{
						sf::Vector2i to_set ( pos() + ((sf::Vector2i)size())/2 );
						if( sf::Mouse::getPosition() == to_set ) break;
						sf::Mouse::setPosition( to_set );
					}
				break;
				default: break;//warning suppression
			}

			mapSFEventToEventHandlerEvents(sfe, event_backlog);
		}
		else if(event_filter.pass(event_backlog.front()) )
		{
			event_filter.update(event_backlog.front());
			e = event_backlog.front();
			event_backlog.pop_front();
			return true;
		}
		else event_backlog.pop_front();
	}
}

sf::Vector2i SFMLWindow::pos(){ return w.getPosition(); }
void SFMLWindow::setPos( sf::Vector2i pos ) { w.setPosition(pos); }
sf::Vector2u SFMLWindow::size(){ return w.getSize(); }
void SFMLWindow::setMouseCursorVisible( bool b ) { w.setMouseCursorVisible(b); }
void SFMLWindow::lockMouse(bool b){ mouse_locked = b; }
