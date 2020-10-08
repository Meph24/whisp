#include "../src/GeoFeatures.hpp"

#include <gtest/gtest.h>

#include <vector>

using std::vector;

struct test_GeoFeatures : public ::testing::Test
{
	vector<Vertex> vertices;

	test_GeoFeatures()
		: vertices{vec3(0,0,0), vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), vec3(1,0,0), vec3(1,1,0)}
	{
	}
};

TEST_F(test_GeoFeatures, EdgeRef)
{
	EdgeRef er (0,1);
	Edge expected = {vec3(0,0,0), vec3(1,0,0)};
	EXPECT_EQ	(	expected,
					er.edge(vertices.begin(), vertices.end())
				);
	EXPECT_EQ(0,er[0]);
	EXPECT_EQ(1,er[1]);
}

TEST_F(test_GeoFeatures, FaceRef)
{
	FaceRef fr (0,1,3);
	Face expected = {vec3(0,0,0), vec3(1,0,0), vec3(0,0,1)};
	EXPECT_EQ	(	expected,
					fr.face(vertices.begin(), vertices.end())
				);
	EXPECT_EQ(0,fr[0]);
	EXPECT_EQ(1,fr[1]);
	EXPECT_EQ(3,fr[2]);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
