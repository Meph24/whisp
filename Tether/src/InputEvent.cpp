#include "InputEvent.hpp"

const int InputEvent::lowid_bits = 10;

InputEvent::Filter::Filter()
	: previous_value( ((unsigned int) NUM_EVENTTYPES) << lowid_bits, 0.0f )
{}

bool InputEvent::Filter::pass(const InputEvent& e)
{
	switch (e.id)
	{
	case 4:
		//text entered events
		return true;			
		break;

		//Mousewheel 1 events have to be excluded due to no reset event(value = 0) will be sent
	case 2050:
		return true;
		break;

		//Mousewheel 1 events have to be excluded due to no reset event(value = 0) will be sent
	case 2051:
		return true;
		break;
	default:
		if (previous_value[e.id] == e.value) return false;
		return true;
		break;
	}
}

void InputEvent::Filter::update(const InputEvent& e)
{
	previous_value[e.id] = e.value;
}

InputEvent::InputEvent(Type type, int lowID, float value)
	: id( (type << lowid_bits) | lowID )
	, value (value)
 {}
