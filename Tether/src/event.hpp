#ifndef EVENT_HPP
#     define EVENT_HPP

#include <vector>

#include "time.hpp"

using std::vector;

typedef int EventId;

namespace InputEventSpec
{
	enum EventType
	{
		None = -1,
		Window = 0,
		Keyboard,
		Mouse,
		Joystick,
		Misc,

		NUM_EVENTTYPES
	};

	static const int event_ids_per_type = 1024;

	//mouse has movement, wheels and key events so we give them each a slice of the id-space
	//
	static const int mouse_movement_offset = 0;
	static const int mouse_wheel_offset = event_ids_per_type / 4;
	static const int mouse_button_offset = event_ids_per_type / 2;

	//multiple joysticks can be connected
	static const int joystick_number_supported = 16;
	static const int joystick_id_offset = event_ids_per_type / joystick_number_supported;
	//joysticks have analogue inputs and buttons
	static const int joystick_movement_offset = 0;
	static const int joystick_button_offset = 16;

	static EventId eventId(EventType eventtype, unsigned int offset)
	{
		if(eventtype >= 0 && offset < event_ids_per_type)
			return eventtype*event_ids_per_type + offset;
		return -1;
	}
}


struct InputEvent
{
	EventId id;
	float value;
	WallClock::time_point time;

	InputEvent(EventId id, float value, WallClock::time_point time)
		: id(id), value(value), time(time) {}

	friend std::ostream& operator<<(std::ostream& os, const InputEvent& e)
	{
		os << "(ID:" << e.id << "|Time:" << e.time.time_since_epoch().count() << "|Value:" << e.value << ')';
		return os;
	}
};

struct InputEventSource
{
	virtual bool pollNext(InputEvent&) = 0;
	virtual unsigned long long polled() const = 0;

	struct Iterator : public std::iterator<std::input_iterator_tag, InputEvent>
	{
		InputEventSource* source;
		InputEvent e;
		unsigned long long c;
		bool isenditerator;

	public:
		Iterator(InputEventSource* source, bool enditerator = false)
			: source(source)
			, e(-1, 0.0f, WallClock::time_point())
			, isenditerator(enditerator)
		{
			if(!isenditerator)
			{
				if(!source->pollNext(e)) isenditerator = true;
				else c = source->polled();
			}
		}
		Iterator(const Iterator& other) = default;
		Iterator& operator= (const Iterator&) = default;
		~Iterator() = default;

		//prae
		Iterator& operator++()
		{
			//the iterator is invalidated (set to end) if it is not sync 
			//	with the source on this advancement attempt
			if(c != source->polled()) isenditerator = true;
			else
			{
				if(!source->pollNext(e)) isenditerator = true;
				else c = source->polled();
			}
			return *this;
		}
		//post
		Iterator operator++(int)
		{
			Iterator newiter = *this;
			++(*this);
			return newiter;
		}

		InputEvent& operator*(){ return e; }
		InputEvent& operator->(){ return e; }

		bool operator== (const Iterator& other) const
		{
			return (isenditerator && other.isenditerator)
					|| (c == other.c);
		}

		bool operator!= (const Iterator& other) const
		{
			return !(*this == other);
		}
	};
	
	Iterator begin()
	{
		return Iterator(this);
	}
	Iterator end()
	{
		return Iterator(this, true);
	}
};

#include <SFML/Window.hpp>

class SFMLInputEventSource : public InputEventSource
{
	sf::Window& w;
	const WallClock& wallclock;

	vector<InputEvent> backlog;

	unsigned long long event_poll_counter;

public:
	SFMLInputEventSource(sf::Window& w, const WallClock& wallclock)
	: w(w), wallclock(wallclock), event_poll_counter(0) 	{}

	static vector<InputEvent> mapSFEventToInputEvents( const sf::Event& sfe, const WallClock::time_point& poll_time)
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
	

	bool pollNext(InputEvent& e_out)
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
	};

	unsigned long long polled() const{ return event_poll_counter; }
};

class Window{};
struct SFMLWindow : public Window
{
	SFMLInputEventSource inputeventsource;
};
#endif /* EVENT_HPP */
