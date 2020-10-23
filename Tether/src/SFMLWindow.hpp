#ifndef SFMLWINDOW_HPP
#     define SFMLWINDOW_HPP

#include "Window.hpp"
#include <SFML/Window.hpp>
#include <string>

#include "EventHandler.h"
#include "Buffer.h"

using std::string;

struct SFMLWindow : public Window, public EventSource
{
	sf::Window w;
	
	bool isOpen() const;
	void close();
	void display(); 

	float width() const;
	float height() const;
	sf::Vector2i pos(){ return w.getPosition(); }
	virtual void setPos( sf::Vector2i pos ) { w.setPosition(pos); }
	sf::Vector2u size(){ return w.getSize(); };
	void activate();

	vector<InputEvent> backlog;

	unsigned long long event_poll_counter;

	SFMLWindow(unsigned int width, unsigned int height, string title, uint32_t style, sf::ContextSettings contextsettings );

	//static vector<InputEvent> mapSFEventToInputEvents( const sf::Event& sfe );

	void mapSFEventToEventHandlerEvents( const sf::Event& sfe, Buffer<EventHandler::event, 4>& eventBuffer);

	bool pollEvent(sf::Event& e)
	{
		return w.pollEvent(e);
	}

	unsigned long long polled() const;

	sf::Window& getSFWindow() { return w; } // TODO Hack remove in future, when Gamemodes do not require a window anymore

	void setMouseCursorVisible( bool b = true ) { w.setMouseCursorVisible(b); }

	void setActive(bool b = true) { w.setActive(b); };
};

#endif /* WINDOW_HPP */
