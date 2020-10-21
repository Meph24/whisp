#ifndef WINDOW_HPP
#     define WINDOW_HPP

#include "event.hpp"

#include <SFML/Window.hpp>
#include "Buffer.h"
#include "EventHandler.h"

struct RenderTarget
{
	virtual void display() = 0;

	virtual float width() const = 0;
	virtual float height() const = 0;
	virtual void setActive(bool b) = 0;
};

struct EventSource
{
	virtual void mapSFEventToEventHandlerEvents( const sf::Event& sfe, Buffer<EventHandler::event, 4>& eventBuffer) = 0;
	virtual bool pollEvent(sf::Event& e) = 0; //TODO the handling of sf events instead of processed own event formats is wrong and should be addressed	
};

struct Window : public RenderTarget
{
	virtual bool isOpen() const = 0;
	virtual void close() = 0;
	virtual sf::Vector2i pos() = 0;
	virtual sf::Vector2u size() = 0;

	virtual sf::Window& getSFWindow() = 0; // TODO Hack remove in future, when Gamemodes do not require a window anymore

	virtual void setMouseCursorVisible( bool b ) = 0;
};

#endif /* WINDOW_HPP */
