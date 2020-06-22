#ifndef WALLCLOCK_HPP
#     define WALLCLOCK_HPP

#include "SessionClock.hpp"
#include "SFMLClock.hpp"

using WallClock = SessionClock<SFMLClock>;
#endif /* WALLCLOCK_HPP */
