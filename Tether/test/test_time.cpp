#include "../src/time.hpp"

#include <gtest/gtest.h>

#include <ratio>

TEST(test_time, WallClock_properties)
{
	//precision
	EXPECT_TRUE( (std::ratio_less_equal<WallClock::period, std::micro>::value) );

	//minimum real time elapsable (before initialization or otherwise)
	//TODO specify exactly
	//right now I think a year is generous, Meph
	typedef duration<int, std::ratio<31556952>> years; //will be added in c++20
	SFMLClock sfmlclock;
	WallClock wc(sfmlclock);
	;
	WallClock::duration d = WallClock::time_point::max() - wc.now();
	EXPECT_GT(d, years(1));
	
	//steadyness
	EXPECT_TRUE(WallClock::is_steady);
}

TEST(test_time, SimClock_properties)
{
	//precision
	EXPECT_TRUE((std::ratio_less_equal<SimClock::period, std::micro>::value));
	
	//minimum simulation time elapsable
	//TODO specify
	
	//steadyness
	EXPECT_TRUE(SimClock::is_steady);
}

//TODO insert test main
