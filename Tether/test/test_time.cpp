#include "../src/time.hpp"

#include <gtest/gtest.h>

#include <chrono>

#include <iostream>

using namespace std::chrono;

TEST(test_FloatSeconds, Float_Accuracy)
{
	nanoseconds ns(1543212345);
	double d = (double)ns.count();
	d /= 1000000000;
	float manually_converted_float_seconds = (float) d;
	FloatSeconds fs = ns;
	ASSERT_EQ(ns, fs);
	//LE = not greater than
	//conversion should not produce an error
	float diff = fs.count() - manually_converted_float_seconds;
	ASSERT_EQ(diff, 0.0f);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
