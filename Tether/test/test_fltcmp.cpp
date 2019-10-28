#include "../src/fltcmp.hpp"

#include <gtest/gtest.h>

TEST(test_fltcmp, default_eps)
{
	float f0 = 3.141592653589793238;
	float f1 = 3.14159;
	float f2 = 3.1415;
	float f3 = 3.14161;

	ASSERT_TRUE(flteq(f0, f1));
	ASSERT_FALSE(flteq(f0, f2));
	ASSERT_FALSE(flteq(f0, f3));
}

TEST(test_fltcmp, custom_epsilon)
{
	float f0 = 3.141592653589793238;
	float f1 = 3.14159;
	float f2 = 3.1415;
	float f3 = 3.14161;

	ASSERT_TRUE(flteqE(f0, f1, 0.00001));
	ASSERT_FALSE(flteqE(f0, f2, 0.00001));
	ASSERT_FALSE(flteqE(f0, f3, 0.00001));
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
