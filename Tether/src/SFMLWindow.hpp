#ifndef SFMLWINDOW_HPP
#     define SFMLWINDOW_HPP

#include "Window.hpp"
#include <SFML/Window.hpp>
#include <string>

using std::string;

struct SFMLWindow : public InputEventSource
{
	sf::Window w;
	
	bool isOpen() const;
	void close();
	void display(); 

	float width() const;
	float height() const;
	void activate();

	const WallClock& wallclock;

	vector<InputEvent> backlog;

	unsigned long long event_poll_counter;

	SFMLWindow(unsigned int width, unsigned int height, string title, const WallClock& wallclock);

	static vector<InputEvent> mapSFEventToInputEvents( const sf::Event& sfe, const WallClock::time_point& poll_time);

	bool pollNext(InputEvent& e_out);

	unsigned long long polled() const;
};

#endif /* WINDOW_HPP */
