#ifndef SESSIONCLOCK_HPP
#     define SESSIONCLOCK_HPP

#include <chrono>
#include "SFMLClock.hpp"

using namespace std::chrono;

template<typename IntervallClock>
class SessionClock
{
public:
	typedef typename IntervallClock::duration duration;
	typedef typename duration::rep rep;
	typedef typename duration::period period;
	typedef typename IntervallClock::time_point time_point;
	static const bool is_steady = IntervallClock::is_steady;
private:
	const IntervallClock& intervall_clock;
	const system_clock& sysclock;
	system_clock::time_point clock_system_epoch;
	time_point clock_epoch;
public:
	SessionClock(const IntervallClock& intervall_clock)
		: intervall_clock(intervall_clock)
		, sysclock(std::chrono::system_clock())
		, clock_system_epoch(sysclock.now())
		, clock_epoch(intervall_clock.now())
	{}

	time_point now () const
	{
		return intervall_clock.now();	
	}

	system_clock::time_point toSystemTime(const time_point& tp)
	{
		duration d = tp - clock_epoch;
		return clock_system_epoch + d;
	}
};
#endif /* SESSIONCLOCK_HPP */
