#ifndef INPUTEVENT_HPP
#define INPUTEVENT_HPP

#include <vector>

using std::vector;

/*
//	A type for InputEvents, which are Abstraktions for User Input
//	ID :	Every event possible must be represented by an unique ID
//	value:	
*/

struct InputEvent
{
	static const int lowid_bits;


	int id;
	float value;

	InputEvent() = default;

	/*
	//	All the types of arrays.
	//	For each type space is created for events
	//	Space for one type = 2^(EVENTHANDLER_LOWID_BITS)
	*/

	enum Type
	{

		Unknown = -1,
		System = 0,
		Keyboard,
		Mouse,
		Joystick,

		NUM_EVENTTYPES
	};
	InputEvent(Type type, int lowiD, float value);

	/*
	//	Filter class which will provide a method to
	//	filter out all events which will not state new values
	//	before handing them on for further
	*/

	class Filter
	{
		vector<float> previous_value;

	public:
		Filter();
		
		void update(const InputEvent& e);
		bool pass(const InputEvent& e);
	};
};

#endif // INPUTEVENT_HPP