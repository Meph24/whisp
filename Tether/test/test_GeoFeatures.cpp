#include "../src/GeoFeatures.hpp"

#include <gtest/gtest.h>

struct test_GeoFeatures : public ::testing:Test
{
	vector<Vertices> vertices;

	test_GeoFeatures()
		: vertices ({{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}, {1,0,0}, {1,1,0}})
	{}
};

TEST_F(test_GeoFeatures, EdgeRef)
{
	EdgeRef er (0,1);
	EXPECT_EQ	(	Edge	(	0, 1,
								0, 0,
								0, 0,
								1, 1	),
					er.edge(vertices.begin(), vertices.end())
				);
	EXPECT_EQ(0,er[0]);
	EXPECT_EQ(1,er[1]);
}

TEST_F(test_GeoFeatures, FaceRef)
{
	EdgeRef er (0,1,3);
	EXPECT_EQ	(	Edge	(	0, 1, 0
								0, 0, 0
								0, 0, 1
								1, 1, 1	),
					er.edge(vertices.begin(), vertices.end())
				);
	EXPECT_EQ(0,er[0]);
	EXPECT_EQ(1,er[1]);
	EXPECT_EQ(2,er[2]);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
