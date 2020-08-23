#include "SFMLWindow.hpp"

bool SFMLWindow::isOpen() const { return w.isOpen(); }
void SFMLWindow::close() { w.close(); }
void SFMLWindow::display() { w.display(); }

float SFMLWindow::width() const { return w.getSize().x; }
float SFMLWindow::height() const { return w.getSize().y; }
void SFMLWindow::activate() { w.setActive(true); }

SFMLWindow::SFMLWindow(unsigned int width, unsigned int height, string title, const WallClock& wallclock)
	: w(sf::VideoMode(400, 300), title)
	, wallclock(wallclock)
	, event_poll_counter(0) {}


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


bool SFMLWindow::pollNext(InputEvent& e_out)
{
	if(!backlog.empty()) 
	{
		e_out = backlog.front();
		event_poll_counter++;
		return true;
	}

	sf::Event sfe;
	vector<InputEvent> mapped_events;
	do
	{
	if(!w.pollEvent(sfe)) return false;

	mapped_events = mapSFEventToInputEvents(sfe, wallclock.now());
	} while (mapped_events.empty());

	backlog.insert(backlog.end(), mapped_events.begin()+1, mapped_events.end());
	e_out = mapped_events.front();
	++event_poll_counter;
	return true;
}

unsigned long long SFMLWindow::polled() const{ return event_poll_counter; }
