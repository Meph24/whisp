#ifndef WINDOW_HPP
#     define WINDOW_HPP

#include "InputEvent.hpp"

struct RenderTarget
{
	virtual void display() = 0;

	virtual float width() const = 0;
	virtual float height() const = 0;
	virtual void setActive(bool b = true) = 0;
};

struct EventSource
{
	virtual bool pollEvent(InputEvent& e) = 0;
};

struct Window : public RenderTarget, public EventSource
{
	virtual bool isOpen() const = 0;
	virtual void close() = 0;
	virtual sf::Vector2i pos() = 0;
	virtual void setPos( sf::Vector2i ) = 0;
	virtual sf::Vector2u size() = 0;

	virtual void setMouseCursorVisible( bool b ) = 0;
	virtual void lockMouse(bool b) = 0;
};

#endif /* WINDOW_HPP */
