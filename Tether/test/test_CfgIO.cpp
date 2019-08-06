#include "../src/CfgIO.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

struct test_CfgIO : public ::testing::Test
{
	fs::path test_dir;
	fs::path in_file;

	fs::path find_test_file( string filename ) const
	{
		fs::path cp = fs::current_path();
		while(cp != cp.root_path())
		{
			fs::path filepath = cp / "test/res" / filename;
			if(	fs::exists(filepath) &&
				fs::is_regular_file(filepath))
			{
				return filepath;
			}
			cp = cp.parent_path();
		}
		return fs::path("");
	}

public:
	test_CfgIO()
	{
		in_file = find_test_file( "test_cfg_in.cfg" );
		if(in_file.empty())
		{
			std::cerr << "Test file <" << in_file << "> could not be found !\n"; 
		}
		test_dir = fs::current_path() / "TEST_CFG";
		fs::create_directories(test_dir);
	}

	~test_CfgIO()
	{
		fs::remove_all(fs::current_path() / "TEST_CFG");
	}
};

TEST_F(test_CfgIO, in_file_found)
{
	ASSERT_FALSE(in_file.empty());
}

TEST_F(test_CfgIO, load)
{
	std::cout << "Testing on file <" << in_file << ">.\n";
	CfgIO cfgio_in (in_file);
	ASSERT_EQ(in_file, cfgio_in.filename());
	Cfg in_cfg = cfgio_in.load();
	{
		const long* lp = in_cfg.getInt("k_int_0");
		ASSERT_TRUE(lp);
		ASSERT_EQ(42, *lp);
	}
	{
		const long* lp = in_cfg.getInt("asdfgh");
		ASSERT_TRUE(lp);
		ASSERT_EQ(1024, *lp);
	}
	{
		const double* dp = in_cfg.getFlt("k_flt_0");
		ASSERT_TRUE(dp);
		ASSERT_EQ(0.1, *dp);
	}
	{
		const double* dp = in_cfg.getFlt("k_flt_1");
		ASSERT_TRUE(dp);
		ASSERT_EQ(3.142, *dp);
	}
	{
		const string* sp = in_cfg.getStr("s0", "k_str_0");
		ASSERT_TRUE(sp);
		ASSERT_EQ("Herbert", *sp);
	}
	{
		const string* sp = in_cfg.getStr("s1", "k_str_0");
		ASSERT_TRUE(sp);
		ASSERT_EQ("Wilhelm", *sp);
	}

	//check nonexisting key
	ASSERT_FALSE(in_cfg.getStr("aaaa"));
	ASSERT_FALSE(in_cfg.getFlt("bbbb"));
	ASSERT_FALSE(in_cfg.getInt("cccc"));
}

TEST_F(test_CfgIO, save)
{
	Cfg out_cfg;
	out_cfg.createSection("s0");
	out_cfg.createSection("s1");
	out_cfg.createSection("s2");
	out_cfg.set("s0", "k_str_0", "string");
	out_cfg.set("s0", "k_str_1", "string\nstring\n");
	out_cfg.set("s0", "k_flt_0", 0.0);
	out_cfg.set("s0", "k_flt_1", 0.125);
	out_cfg.set("s0", "k_int_0", (long)0);
	out_cfg.set("s0", "k_int_1", (long)42);
	out_cfg.set("s1", "k_int_0", (long)1);

	fs::path out_file = test_dir / "test_cfg_out.cfg";
	CfgIO cfgio_out(out_file);
	cfgio_out.save(out_cfg);

	//read back in
	Cfg reread = cfgio_out.load();

	ASSERT_TRUE(reread.getStr("s0", "k_str_0"));
	ASSERT_TRUE(reread.getStr("s0", "k_str_1"));
	ASSERT_TRUE(reread.getFlt("s0", "k_flt_0"));
	ASSERT_TRUE(reread.getFlt("s0", "k_flt_0"));
	ASSERT_TRUE(reread.getInt("s0", "k_int_0"));
	ASSERT_TRUE(reread.getInt("s0", "k_int_1"));
	ASSERT_TRUE(reread.getInt("s1", "k_int_0"));

	ASSERT_EQ("string", *reread.getStr("s0", "k_str_0"));
	ASSERT_EQ("string\nstring\n", *reread.getStr("s0", "k_str_1"));
	ASSERT_EQ(0.0, *reread.getFlt("s0", "k_flt_0"));
	ASSERT_EQ(0.125, *reread.getFlt("s0", "k_flt_1"));
	ASSERT_EQ(0, *reread.getInt("s0", "k_int_0"));
	ASSERT_EQ(42, *reread.getInt("s0", "k_int_1"));
	ASSERT_EQ(1, *reread.getInt("s1", "k_int_0"));
}

TEST(CfgIO_method, isComment)
{
	CfgIO cfgio;

	string comment = "#comment";
	string no_comment = " #no_comment";

	ASSERT_TRUE(cfgio.isComment(comment));
	ASSERT_FALSE(cfgio.isComment(no_comment));
}

TEST(CfgIO_method, isEmpty)
{
	CfgIO cfgio;

	string empty = "";
	string whitespace_chars = " \t\n";
	string non_empty = "Hi there!";

	ASSERT_TRUE(cfgio.isEmpty(empty));
	ASSERT_TRUE(cfgio.isEmpty(whitespace_chars));
	ASSERT_FALSE(cfgio.isEmpty(non_empty));
}

TEST(CfgIO_method, getSectionName)
{
	CfgIO cfgio;

	string sectionname = "[section]";
	string sectionname_whitespace = "   \t [ section  ] ";
	string non_sectionname = "blablablubber";

	ASSERT_EQ("section", cfgio.getSectionName(sectionname));
	ASSERT_EQ("section", cfgio.getSectionName(sectionname_whitespace));
	ASSERT_EQ("", cfgio.getSectionName(non_sectionname));
}


TEST(CfgIO_method, splitKeyValue)
{
	CfgIO cfgio;

	pair<string, string> kv;

	string keyvalue = "key=42";
	string keyvalue_whitespace = "  key  =   43 ";
	string keyvalue_quotes = "multiline_string = \"\"\"";

	kv = cfgio.splitKeyValue(keyvalue_whitespace);
	ASSERT_EQ("key", kv.first);
	ASSERT_EQ("43", kv.second);

	kv = cfgio.splitKeyValue(keyvalue_quotes);
	ASSERT_EQ("multiline_string", kv.first);
	ASSERT_EQ("\"\"\"", kv.second);
}

TEST(CfgIO_method, match)
{
	CfgIO cfgio;

	string i = "42";
	string weird_i = "    42 ";

	ASSERT_EQ("42", cfgio.match(i, CfgIO::re_int));
	ASSERT_EQ("42", cfgio.match(weird_i, CfgIO::re_int));


	string f0 = "  0.";
	string f1 = " .5";
	string f2 = "4.";

	ASSERT_EQ("0.", cfgio.match(f0, cfgio.re_flt));
	ASSERT_EQ(".5", cfgio.match(f1, cfgio.re_flt));
	ASSERT_EQ("4.", cfgio.match(f2, cfgio.re_flt));
}

TEST(CfgIO_method, readFloatstring)
{
	CfgIO cfgio;
	ASSERT_EQ(4.0, cfgio.readFloatstring("4."));
	ASSERT_EQ(0.0, cfgio.readFloatstring("."));
	ASSERT_EQ(3.142, cfgio.readFloatstring("3.142"));
	ASSERT_EQ(0.5, cfgio.readFloatstring(".5"));
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
