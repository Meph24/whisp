#ifndef SFMLWINDOW_HPP
#     define SFMLWINDOW_HPP

#include <deque>
#include <SFML/Window.hpp>
#include <string>

#include "InputDeviceConfigurator.hpp"
#include "InputEvent.hpp"
#include "Window.hpp"

using std::deque;
using std::string;

struct SFMLWindow : public Window
{
	//impl RenderTarget
	float width() const;
	float height() const;
	void display(); 
	void setActive(bool b = true);

	//impl EventSource
	void mapSFEventToEventHandlerEvents( const sf::Event& sfe, deque<InputEvent>& event_backlog);
	deque<InputEvent> event_backlog;
	InputEvent::Filter event_filter;
	bool pollEvent(InputEvent& e);

	//impl Window
	sf::Window w;
	bool isOpen() const;
	void close();
	sf::Vector2i pos();
	virtual void setPos( sf::Vector2i pos );
	sf::Vector2u size();
	void setMouseCursorVisible( bool b = true );
	bool mouse_locked = false;
	void lockMouse(bool b);

	SFMLWindow(unsigned int width, unsigned int height, 
		string title, uint32_t style, sf::ContextSettings contextsettings);
};

#endif /* WINDOW_HPP */
