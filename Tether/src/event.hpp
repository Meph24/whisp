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

	InputEvent();
	InputEvent(EventId, float, WallClock::time_point);

	friend std::ostream& operator<<(std::ostream& os, const InputEvent& e);
};

struct InputEventSource
{
	virtual bool pollNext(InputEvent&) = 0;
	virtual unsigned long long polled() const = 0;

	virtual ~InputEventSource() = default;

	struct Iterator : public std::iterator<std::input_iterator_tag, InputEvent>
	{
		InputEventSource* source;
		InputEvent e;
		unsigned long long c;
		bool isenditerator;

	public:
		Iterator(InputEventSource* source, bool enditerator = false);
		Iterator(const Iterator& other) = default;
		Iterator& operator= (const Iterator&) = default;
		~Iterator() = default;

		//prae
		Iterator& operator++();
		//post
		Iterator operator++(int);
		InputEvent& operator*();
		InputEvent& operator->();

		bool operator== (const Iterator& other) const;
		bool operator!= (const Iterator& other) const;
	};
	
	Iterator begin();
	Iterator end();
};

#include <SFML/Window.hpp>

class SFMLInputEventSource : public InputEventSource
{
	sf::Window& w;
	const WallClock& wallclock;

	vector<InputEvent> backlog;

	unsigned long long event_poll_counter;

public:
	SFMLInputEventSource(sf::Window& w, const WallClock& wallclock);

	static vector<InputEvent> mapSFEventToInputEvents( const sf::Event& sfe, const WallClock::time_point& poll_time);

	bool pollNext(InputEvent& e_out);

	unsigned long long polled() const;
};

struct InputEventInterpreter
{
	virtual void interpret(InputEvent& inputevent) = 0;
};

#endif /* EVENT_HPP */
