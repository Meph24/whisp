#include "../src/RelativeClock.hpp"

#include <gtest/gtest.h>

#include <chrono>

#include <iostream>
#include <thread>

//for random
#include <cstdlib>
#include <ctime>

using namespace std::chrono;

int randomInteger(int min, int max)
{
	return rand() % (max - min) + min;
}

/**
 * @brief A clock that advances its time after parameters given
 *
 * Exists to control advancement of time.
 */
struct TestBaseClock
{
	typedef microseconds duration;
	typedef duration::rep rep;
	typedef duration::period period;
	typedef std::chrono::time_point<TestBaseClock> time_point;
	static const bool is_steady = true; // clock can sample the same time_point twice

	time_point current;
	TestBaseClock()= default; 
	void advance(duration d){ current += d; }
	time_point now() const { return current; }
};

typedef RelativeClock<TestBaseClock> RC;

TEST(test_RelativeClock, different_targetrates)
{
	TestBaseClock tbc;

	microseconds expected = 1500us;

	//no constraints should cause an effect
	RC c1(tbc, 1.0f, seconds(1), seconds(5));
	auto c1_0 = c1.now();
	
	tbc.advance(expected);

	auto c1_1 = c1.now();
	RC::duration diff1 = c1_1 - c1_0;
	RC::duration abs1 = abs(diff1 - expected);
	EXPECT_LT(abs1, 1us)  <<
		"distance to expected : " << duration_cast<nanoseconds>(abs1).count() << " ns | diff:" << duration_cast<nanoseconds>(diff1).count() << " ns";


	RC c2(tbc, 2.0f, seconds(1), seconds(5));
	auto c2_0 = c2.now();
	tbc.advance(expected);
	auto c2_1 = c2.now();
	RC::duration diff2 = c2_1 - c2_0;
	RC::duration abs2 = abs(diff2 - expected*2 );
	EXPECT_LT( abs2 , 1us )	<< 
		"distance to expected : " << duration_cast<nanoseconds>(abs2).count() << " ns | diff:" << duration_cast<nanoseconds>(diff2).count() << " ns";


	RC c05(tbc, 0.5f, seconds(1), seconds(5));
	auto c05_0 = c05.now();
	tbc.advance(expected);
	auto c05_1 = c05.now();
	RC::duration us05 = c05_1 - c05_0;
	us05 = abs(us05- expected/2 );
	EXPECT_LT(us05, 1us) << duration_cast<nanoseconds>(us05).count() << " ns";
}

TEST(test_RelativeClock, max_base_time)
{
	TestBaseClock tbc;
	microseconds max_base_time = 2ms;
	RC c1(tbc, 1.0f, max_base_time, seconds(5));

	auto c1_0 = c1.now();

	//default behavior when max_base_time isn't reached
	//at this first duration, we do not reach the max_base_time
	microseconds first_wait_duration = max_base_time/2;
	ASSERT_LT(first_wait_duration, max_base_time);
	tbc.advance(first_wait_duration);
	auto c1_1 = c1.now();
	RC::duration diff = c1_1 - c1_0;


	EXPECT_LT(abs(diff - first_wait_duration), microseconds(1)) 
		<< "actual : " << duration_cast<nanoseconds>(abs(diff - first_wait_duration)).count() << " ns";

	//since the clock wasn't ticked simtime should stop at another half of maxbasetime
	// since in the previous duration we elapsed the first half of it, we now do the second half, and a bit more
	microseconds second_wait_duration = first_wait_duration + microseconds(100);
	tbc.advance(second_wait_duration);
	auto c1_2 = c1.now();
	diff = c1_2 - c1_0;
	EXPECT_LT(abs(diff - max_base_time), microseconds(1))
		<< "actual : " << duration_cast<nanoseconds>(abs(diff - max_base_time)).count() << " ns";
	diff = c1_2 - c1_1;
	EXPECT_GT(abs(diff - second_wait_duration), microseconds(1))
		<< "actual : " << duration_cast<nanoseconds>(abs(diff - second_wait_duration)).count() << " ns";
	EXPECT_LT(abs(diff - first_wait_duration), microseconds(1))
		<< "actual : " << duration_cast<nanoseconds>(abs(diff - first_wait_duration)).count() << " ns";

	//the clock does not move on subsequent samples
	EXPECT_EQ(c1.now(), c1_2);	

	//now we tick the clock making it able to run again as it now is stuck
	auto c1_ticktime = c1.tick();
	//the from the tick registered time should be the same timepoint we stopped
	EXPECT_EQ(c1_ticktime, c1_2);
	//since we could not keep up last time around and the base clock has advanced too far, we could not hold the target rate of the clock
	//hence the last rate of the clock should now be < 1.0f and our now() calls are now not realtime anymore, since the clock now "runs slower"
	EXPECT_LT(c1.lastRate(), 1.0f);


	microseconds third_wait_duration = 200us;
	auto c1_3 = c1.now();
	EXPECT_EQ(c1_ticktime, c1_3);
	tbc.advance(third_wait_duration);
	auto c1_4 = c1.now();
	diff = c1_4 - c1_3;
	EXPECT_LT(diff , third_wait_duration);
};

TEST(test_RelativeClock, max_rel_time_default)
{
	TestBaseClock tbc;
	microseconds max_rel_time = 1ms;
	float clockrate = 5.0f;
	RC c5(tbc, clockrate, seconds(1), milliseconds(1));
	RC::time_point c5_0 = c5.now();

	//this should trigger no constraints
	microseconds first_wait_time = duration_cast<microseconds> (max_rel_time / clockrate / 2);
	tbc.advance(first_wait_time);
	RC::time_point c5_1 = c5.now();
	EXPECT_LT(abs((c5_1 - c5_0) - max_rel_time/2), 1us);

	//since we have not ticked the clock, we hit the simulation time constraint here
	microseconds second_wait_time = duration_cast<microseconds>((max_rel_time / clockrate) - first_wait_time + 200us);
	tbc.advance(second_wait_time);
	RC::time_point c5_2 = c5.now();
	microseconds diff = c5_2 - c5_0;
	EXPECT_LT(abs(diff - max_rel_time), 1us);

	//an because
	ASSERT_NE((first_wait_time + second_wait_time) * clockrate, max_rel_time);
	//the following is also correct
	EXPECT_GT(abs(diff - (first_wait_time + second_wait_time)), 1us);

	//the clock does not move on subsequent samples
	EXPECT_EQ(c5.now(), c5_2);

	//up until we tick it
	RC::time_point c5_ticktime = c5.tick();
	//the from the tick registered time should be the same timepoint we stopped
	EXPECT_EQ(c5_ticktime, c5_2);

	RC::time_point c5_3 = c5.now();

	//since we have not advanced the base_clock, yet, all subsequent samples should be the same, with a real clock this next line is of course false
	EXPECT_EQ(c5_ticktime, c5_3);

	//the clock advances again, but has a different rate, because we violated constraints (so it is not the same as before)
	tbc.advance(first_wait_time);
	RC::time_point c5_4 = c5.now();
	EXPECT_GT(c5_4 - c5_3, 1us);
}

TEST(test_RelativeClock, constraint_rate_changes)
{
	TestBaseClock tbc;
	RC c(tbc, 1.0f, 50ms, 100ms);

	EXPECT_EQ(c.lastRate() , 1.0f);
	
	tbc.advance(10ms);

	c.tick();
	//if no constraint is hit, no change
	EXPECT_EQ(c.lastRate(), 1.0f);

	tbc.advance(60ms);
	//if a constraint hits, it changes
	c.tick();
	EXPECT_EQ(c.lastRate(), 50/60);


}

TEST(test_RelativeClock, new_rate)
{
	TestBaseClock tbc;
	RC c1(tbc, 1.0f, milliseconds(10), milliseconds(100));
	auto c1_0 = c1.now();
	c1.setNextTargetRate(2.0f);

	microseconds c1_expected_0 = milliseconds(1);
	tbc.advance(c1_expected_0);
	auto c1_1 = c1.tick();
	RC::duration c1_diff = c1_1 - c1_0;
	EXPECT_LT(abs(c1_diff - c1_expected_0), microseconds(1));
	
	// now on rate 2.0
	tbc.advance(milliseconds(1));
	auto c1_2 = c1.tick();
	microseconds c1_expected_1 = milliseconds(2);
	c1_diff = c1_2 - c1_1;
	EXPECT_LT(abs(c1_diff - c1_expected_1), microseconds(1));
}

TEST(test_RelativeClock, new_max_base_time)
{
	TestBaseClock tbc;
	RC c1(tbc, 1.0f, milliseconds(10), milliseconds(100));
	auto c1_0 = c1.now();
	c1.setNextMaxBaseTime(milliseconds(5));

	microseconds c1_expected_0 = milliseconds(7);
	tbc.advance(milliseconds(7));
	auto c1_1 = c1.tick();
	RC::duration c1_diff = c1_1 - c1_0;
	EXPECT_LT(abs(c1_diff - c1_expected_0), microseconds(1));
	
	// now constraint due to lower max_base_time
	microseconds c1_expected_1 = milliseconds(5);
	tbc.advance(milliseconds(7));
	auto c1_2 = c1.tick();
	c1_diff = c1_2 - c1_1;
	EXPECT_LT(abs(c1_diff - c1_expected_1), microseconds(1));
}

TEST(test_RelativeClock, new_max_rel_time)
{
	TestBaseClock tbc;
	RC c1(tbc, 1.0f, milliseconds(20), milliseconds(10));
	auto c1_0 = c1.tick();
	c1.setNextMaxRelTime(milliseconds(5));

	tbc.advance(milliseconds(7));
	auto c1_1 = c1.tick();
	microseconds c1_expected_0 = milliseconds(7);
	RC::duration c1_diff = c1_1 - c1_0;
	EXPECT_LT(abs(c1_diff - c1_expected_0), microseconds(1));
	
	// now constraint due to lower max_base_time
	tbc.advance(milliseconds(7));
	auto c1_2 = c1.tick();
	microseconds c1_expected_1 = milliseconds(5);
	c1_diff = c1_2 - c1_1;
	EXPECT_LT(abs(c1_diff - c1_expected_1), microseconds(1));
}

int main (int argc , char** argv)
{
	srand(time(NULL));

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
