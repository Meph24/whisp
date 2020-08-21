#ifndef SIMCLOCK_HPP
#     define SIMCLOCK_HPP

#include "RelativeClock.hpp"
#include "WallClock.hpp"

typedef RelativeClock<WallClock> SimClock;

#endif /* SIMCLOCK_HPP */
