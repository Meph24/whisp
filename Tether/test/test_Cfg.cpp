#include "../src/Cfg.hpp"

#include <gtest/gtest.h>

struct test_Cfg : public ::testing::Test
{
	Cfg cfg;
public:
	test_Cfg()
	{
			cfg.createSection("s0");
			cfg.createSection("s1");
			cfg.createSection("s2");
			cfg.set("s0", "k_str_0", "string");
			cfg.set("s0", "k_str_1", "string\nstring\n");
			cfg.set("s0", "k_flt_0", 0.0);
			cfg.set("s0", "k_flt_1", 0.125);
			cfg.set("s0", "k_int_0", (long)0);
			cfg.set("s0", "k_int_1", (long)42);
			cfg.set("s1", "k_int_0", (long)1);
	}

	~test_Cfg()
	{
	
	}
};

TEST_F(test_Cfg, direct_setters)
{
	//tested in fixture
	ASSERT_TRUE(true);
}

TEST_F(test_Cfg, direct_getters)
{
	{
		//single line string
		const string* sp = cfg.getStr("s0", "k_str_0");
		ASSERT_TRUE(sp);
		ASSERT_EQ("string", *sp);
	}
	{	
		//mulit line string
		const string* sp = cfg.getStr("s0", "k_str_1");
		ASSERT_TRUE(sp);
		ASSERT_EQ("string\nstring\n", *sp);
	}
	{	
		//float
		const double* dp = cfg.getFlt("s0", "k_flt_1");
		ASSERT_TRUE(dp);
		ASSERT_EQ(0.125, *dp);
	}
	{
		//int
		const long* lp = cfg.getInt("s0", "k_int_1");
		ASSERT_TRUE(lp);
		ASSERT_EQ(42, *lp);
	}
	{
		//different section int
		const long* lp = cfg.getInt("s1", "k_int_0");
		ASSERT_TRUE(lp);
		ASSERT_EQ(1, *lp);
	}
}

TEST_F(test_Cfg, section_getters)
{
	Cfg::Section sec = cfg.getSection("s0");
	{
		//single line string
		const string* sp = sec.getStr("k_str_0");
		ASSERT_TRUE(sp);
		ASSERT_EQ("string", *sp);
	}
	{
		//mulit line string
		const string* sp = sec.getStr("k_str_1");
		ASSERT_TRUE(sp);
		ASSERT_EQ("string\nstring\n", *sp);
	}
	{
		//float
		const double* dp = sec.getFlt("k_flt_1");
		ASSERT_TRUE(dp);
		ASSERT_EQ(0.125, *dp);
	}
	{
		//int
		const long* dp = sec.getInt("k_int_1");
		ASSERT_TRUE(dp);
		ASSERT_EQ(42, *dp);
	}
}

TEST_F(test_Cfg, unsafe_getters)
{
	//this key is unique among sections
	const string* sp = cfg.getStr("k_str_0");
	ASSERT_TRUE(sp);
	ASSERT_EQ("string", *sp);

	std::set<long> test_set ({0, 1});
	//this key is not unique among sections
	//therefor the result can be one of the ones specified in
	//the test_set
	ASSERT_TRUE(test_set.count(*cfg.getInt("k_int_0")));
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
