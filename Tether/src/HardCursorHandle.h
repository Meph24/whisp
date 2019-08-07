#pragma once

#include <SFML/Window.hpp>

class HardCursorHandle
{

	/*
	//	position where the hardcursor will be hold in place in pixels in the window
	//	the actual lockposition however must be relative to the dektop and therefor must be calculated
	*/
	sf::Vector2i lockposition; //in pixels relative to window
	sf::Vector2i desklockposition; //in pixels relative to desktop

	/*
	// saves the locked or unlocked state
	*/
	bool islocked;

	/*
	//	to get position data of the window
	*/
	sf::Window& w;
public:

	/*
	//	The window must be passed in to provide it's position data
	//	Sets the lockposition in the center of the window
	*/
	HardCursorHandle(sf::Window& w);

	/*
	//	sets the lockPositon in pixels relative to the window
	//	in default this is set on the center of the window
	*/
	void setLockPosition(const sf::Vector2i lockposition);
	
	/*
	//	access to the state of the hardcursor being locked in place
	//	pass in true to lock and false to unlock
	*/
	void setLocked(bool locked);

	/*
	//	returns the state of the cursor
	// true = locked
	// false = unlocked
	*/
	bool isLocked() const;

	/*
	//	updateLockPositon() provides a method to update the actual 
	//	position of the lock in cases of a window resizeing or moving
	*/
	void updateLockPosition();


	/*
	//	If the cursor moves this method is to be called to set the cursor back if necessary
	//	To put directly in event handling (before the events get actually handled is best)
	//	The method may corrupt the event by setting its type to an unhandled one (sf::Event::EventType::Count)
	//	thereby filtering events because other event handlers cant handle this type of event
	*/
	void cursorMoved(sf::Event& e);

};

