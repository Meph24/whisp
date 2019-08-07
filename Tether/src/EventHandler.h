#pragma once


/*
//	Space for the lowerID parts
*/
#define EVENTHANDLER_LOWID_BITS 10


class EventHandler
{
public:

	/*
	//	A type for events
	//	ID :	Every event possible must be represented by an unique ID
	//	value:	
	*/

	struct event
	{
		int ID;
		float value;
	};

	/*
	//	All the types of arrays.
	//	For each type space is created for events
	//	Space for one type = 2^(EVENTHANDLER_LOWID_BITS)
	*/

	enum eventType
	{
		Unknown = -1,
		System = 0,
		Keyboard,
		Mouse,
		Joystick,

		//insert additonal above
		NUM_EVENTTYPES
	};
private:

	/*
	//	Filter class which will provide a method to
	//	filter out all events which will not state new values
	//	before handing them on for further
	*/

	class Filter
	{
		float* prevValue;

	public:
		Filter();
		~Filter();
		
		void updateFilter(event e);
		bool filter(event e);
	}Filter;

	/*
	//	Method to pass on the event to further use
	*/

	void sendOn(event e);
public:

	/*
	//	The method to get events handled in further processes
	//	Includes filtering of events, which do not bring new values
	*/

	void handle(event e);

	/*
	//	Method to ease creation of events from type and lower ID Parts
	//	Returns an event of the EventHandler definition
	//	Note: this method contains no further processes
	*/

	event createEvent(eventType type, int lowiD, float value);

};

