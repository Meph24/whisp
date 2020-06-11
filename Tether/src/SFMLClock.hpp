#ifndef SFMLCLOCK_HPP
#     define SFMLCLOCK_HPP

#include <chrono>

#include <SFML/System.hpp>


using std::chrono::microseconds;
using std::chrono::duration;

struct SFMLClock
{
	typedef microseconds duration;
	typedef duration::rep rep;
	typedef duration::period period;
	typedef std::chrono::time_point<SFMLClock, duration> time_point;
	static const bool is_steady = true;

	static sf::Clock sfmlclock;

	static time_point now()
	{
		return time_point(microseconds(sfmlclock.getElapsedTime().asMicroseconds())); 
	}
};

sf::Clock SFMLClock::sfmlclock = sf::Clock();

#endif /* SFMLCLOCK_HPP */
