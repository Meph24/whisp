#include "event.hpp"

namespace InputEventSpec{
EventId eventId(EventType eventtype, unsigned int offset)
{
	if(eventtype >= 0 && offset < event_ids_per_type)
		return eventtype*event_ids_per_type + offset;
	return -1;
}
} /* namespace InputEventSpec */


InputEvent::InputEvent()
		: id(-1), value(0.0f), time(WallClock::time_point()) {}

InputEvent::InputEvent(EventId id, float value, WallClock::time_point time)
		: id(id), value(value), time(time) {}

std::ostream& operator<<(std::ostream& os, const InputEvent& e)
{
	os << "(ID:" << e.id << "|Time:" << e.time.time_since_epoch().count() << "|Value:" << e.value << ')';
	return os;
}

InputEventSource::Iterator::Iterator(InputEventSource* source, bool enditerator)
	: source(source)
	, e()
	, c(source->polled())
	, isenditerator(enditerator)
{}

//prae
InputEventSource::Iterator& InputEventSource::Iterator::operator++()
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
InputEventSource::Iterator InputEventSource::Iterator::operator++(int)
{
	Iterator newiter = *this;
	++(*this);
	return newiter;
}

InputEvent& InputEventSource::Iterator::operator*(){ return e; }
InputEvent& InputEventSource::Iterator::operator->(){ return e; }

bool InputEventSource::Iterator::operator== (const Iterator& other) const
{
	return (isenditerator && other.isenditerator)
			|| (c == other.c);
}

bool InputEventSource::Iterator::operator!= (const Iterator& other) const
{
	return !(*this == other);
}

InputEventSource::Iterator InputEventSource::begin()
{
	return Iterator(this);
}
InputEventSource::Iterator InputEventSource::end()
{
	return Iterator(this, true);
}
