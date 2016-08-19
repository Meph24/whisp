#include "HardCursorHandle.h"


// _test_begin
#include <iostream>
// _test_end


HardCursorHandle::HardCursorHandle(sf::Window& w) 
	:
	w(w),
	islocked(false)
{
	updateLockPosition();
}

void HardCursorHandle::setLockPosition(sf::Vector2i lockposition)
{
	this->lockposition = lockposition;
}

void HardCursorHandle::updateLockPosition()
{
	sf::Vector2u size = w.getSize();
	setLockPosition(sf::Vector2i(size.x / 2, size.y / 2));
	desklockposition = w.getPosition() + lockposition;
}

void HardCursorHandle::setLocked(bool islocked)
{
	if (islocked)
	{
		sf::Event e;
		e.mouseMove.x = lockposition.x;
		e.mouseMove.y = lockposition.y;
		cursorMoved(e);
	}
	this->islocked = islocked;
	
}

bool HardCursorHandle::isLocked()const
{
	return islocked;
}

void HardCursorHandle::cursorMoved(sf::Event& e)
{
	sf::Vector2i v (e.mouseMove.x, e.mouseMove.y);	
	if (islocked)
	{
		if (v == lockposition) {
			// here the event gets corrupted for the other event handlers
			e.type = sf::Event::EventType::Count;	
			return; 
		}
		sf::Mouse::setPosition(desklockposition);
	}
}