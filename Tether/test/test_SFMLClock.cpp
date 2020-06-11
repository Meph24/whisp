#include "../src/SFMLClock.hpp"

#include <gtest/gtest.h>

#include <chrono>

#include <iostream>
#include <thread>

using namespace std::chrono;

struct test_SFMLClock : ::testing::Test
{
	SFMLClock c;
};

TEST_F(test_SFMLClock, classic_usage)
{
	SFMLClock::time_point t0 = c.now();
	std::this_thread::sleep_for(microseconds(2500));
	SFMLClock::time_point t1 = c.now();

	microseconds ms = t1 - t0;

	int64_t diff = 2500 - ms.count();

	//inaccuracies with sleeping and sampling the clock are ok
	//but unreasonable values are to be detected by this test
	ASSERT_LE(diff, 200);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
