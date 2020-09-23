#include "../src/gjk.hpp"


#include <vector>
#include <utility>
#include "../src/GeoFeatures.hpp"
#include "../src/glmutils.hpp"

#include <gtest/gtest.h>

using namespace gjk;

using std::pair;
using std::vector;

TEST(MinkowskiPoint, equals)
{
	MinkowskiPoint mp0(vec3(0.0f), 1, 1);
	MinkowskiPoint mp1(vec3(0.5f), 1, 1);
	MinkowskiPoint mp2(vec3(0.0f), 2, 1);


	ASSERT_EQ(mp0, mp1);

	//equality only tested with the indices given
	//therefore inequality between
	ASSERT_NE(mp0, mp2);
}

TEST(MinkowskiPoint, implicit_operators)
{
	MinkowskiPoint mp0 (vec3(1.0f, 1.0f, 1.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(0.5f, 0.5f, 0.5f), 0, 1);

	EXPECT_EQ(vec3(1.5f, 1.5f, 1.5f), mp0+mp1);
	EXPECT_EQ(vec3(0.5f, 0.5f, 0.5f), mp0-mp1);
}

TEST(test_gjk, MinkowskiGenerator)
{
	vector<Vertex> vertices0 = { Vertex(1.0f, 1.0f, 1.0f, 1.0f),
									  Vertex(-0.75f, -0.75f, 0.75f, 1.0f)};
	vector<Vertex> vertices1 = { Vertex(0.5f, 0.5f, 0.5f, 1.0f),
									  Vertex(-0.25f, -0.25f, -0.25f, 1.0f)	};
	vector<unsigned int> indices0 = {0};
	vector<unsigned int> indices1 = {0, 1};

	vec3 relative_position (0.5f, 0.5f, 0.5f);

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);


	vector<MinkowskiPoint> expect ({	MinkowskiPoint(vec3(0.0f, 0.0f, 0.0f), 0, 0),
										MinkowskiPoint(vec3(0.75f, 0.75f, 0.75f), 0, 1),
									});

	vector<MinkowskiPoint> result(mg.begin(), mg.end());

	EXPECT_EQ(expect, result);

	EXPECT_EQ(Vertex(1.0f, 1.0f, 1.0f, 1.0f), (*(vertices0.begin() + result[0].i0)));
	EXPECT_EQ(Vertex(0.5f, 0.5f, 0.5f, 1.0f), (*(vertices1.begin() + result[0].i1)));


	EXPECT_EQ(Vertex(1.0f, 1.0f, 1.0f, 1.0f), (*(vertices0.begin() + result[1].i0)));
	EXPECT_EQ(Vertex(-0.25f, -0.25f, -0.25f, 1.0f), (*(vertices1.begin() + result[1].i1)));
}

TEST(test_gjk, support)
{
	//for simplicitly we use only 2d
	vector<Vertex> vertices0 = {	Vertex(1.0f, 0.0f, 0.0f, 1.0f),
									Vertex(0.0f, 0.0f, 0.0f, 1.0f),
									Vertex(0.5f, 1.0f, 0.0f, 1.0f)
								};

	vector<Vertex> vertices1 = {	Vertex(1.0f, 0.0f, 0.0f, 1.0f),
									Vertex(0.0f, 0.0f, 0.0f, 1.0f),
									Vertex(0.5f, 1.0f, 0.0f, 1.0f)
								};

	vector<unsigned int> indices0 = {0,1,2};
	vector<unsigned int> indices1 = {0,1,2};

	vec3 relative_position (0.5f, 0.5f, 0.0f);

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);

	gjk::MinkowskiPoint expect_point, result;

	expect_point =  MinkowskiPoint(vec3(0.0f, 0.5f, 0.0f), 2, 1);
	result = maxSupport(mg.begin(), mg.end(), vec3(1.0f, 1.0f, 0.0f));

	ASSERT_EQ(expect_point, result) << glm::to_string(result) << ", " << result.i0 << ", " << result.i1 << " ;expected : " << glm::to_string(expect_point) << expect_point.i0 << ", " << expect_point.i1 ;

	
	expect_point =  gjk::MinkowskiPoint(vec3(-1.5f, -0.5f, 0.0f), 1, 0);
	result = gjk::maxSupport(mg.begin(), mg.end(), vec3(-1.0f, 0.0f, 0.0f));

	ASSERT_EQ(expect_point, result) << glm::to_string(result) << ", " << result.i0 << ", " << result.i1 << " ;expected : " << glm::to_string(expect_point) << expect_point.i0 << ", " << expect_point.i1 ;

}

TEST(gjk, parallel)
{
	vec3 v1(1.0f, 1.0f, 0.0f);
	vec3 v2(2.5f, 2.5f, 0.0f);
	vec3 v3(2.4f, 2.5f, 0.0f);

	ASSERT_TRUE(parallel(v1, v2));
	ASSERT_FALSE(parallel(v1, v3));
}

TEST(gjk, sameDirection)
{
	vec3 up (0.0f, 1.0f, 0.0f);
	vec3 down = -up;
	vec3 right (1.0f, 0.0f, 0.0f);
	vec3 forward(0.0f, 0.0f, 1.0f);
	vec3 upright(1.0f, 1.0f, 0.0f);

	EXPECT_TRUE(sameDirection(up, up));
	EXPECT_TRUE(sameDirection(up, upright));
	EXPECT_TRUE(sameDirection(right, upright));
	EXPECT_FALSE(sameDirection(up, right));
	EXPECT_FALSE(sameDirection(right, forward));
	EXPECT_FALSE(sameDirection(forward, upright));
	EXPECT_FALSE(sameDirection(upright, forward));
	EXPECT_FALSE(sameDirection(down, up));
	EXPECT_FALSE(sameDirection(up, down));
}

TEST(gjk, doPoint)
{
	MinkowskiPoint mp (vec3(1.0f, 0.5f, 0.25f), 0, 1);
	std::vector<MinkowskiPoint> supports
		= {mp};
	vec3 direction;

	EXPECT_FALSE(doPoint(supports, direction));
	EXPECT_EQ(direction, -(vec3)supports[0]);
	EXPECT_EQ(1, supports.size());
	EXPECT_EQ(supports[0], mp);
}

TEST(gjk, doLine_line_case)
{
	MinkowskiPoint mp0 (vec3(1.0f, -0.5f, 0.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(1.0f, 0.5f, 0.0f), 0, 2);
	std::vector<MinkowskiPoint> supports = {mp0, mp1};
	
	vec3 direction;

	EXPECT_FALSE(doLine(supports, direction));
	EXPECT_EQ(2, supports.size());
	EXPECT_EQ(mp0, supports[0]);
	EXPECT_EQ(mp1, supports[1]);
	EXPECT_TRUE(sameDirection(direction, vec3(-1.0f, 0.0f, 0.0f)));
}

TEST(gjk, doLine_point_case)
{
	MinkowskiPoint mp0 (vec3(1.0f, 1.5f, 0.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(1.0f, 1.0f, 0.0f), 0, 2);
	std::vector<MinkowskiPoint> supports = {mp0, mp1};

	vec3 direction;

	EXPECT_FALSE(doLine(supports, direction));
	EXPECT_EQ(1, supports.size());
	EXPECT_EQ(mp1, supports[0]);
	EXPECT_TRUE(parallel(vec3(-1.0f, -1.0f, 0.0f), direction)) << "direction : <" << glm::to_string(direction) << ">";
}

TEST(gjk, doTriangle_point_case_0)
{
	MinkowskiPoint mp2 (vec3(1.0f, 0.0f, 0.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(2.0f, 0.0f, 0.0f), 0, 1);
	MinkowskiPoint mp0 (vec3(1.5f, 1.0f, 0.0f), 0, 1);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2};

	vec3 direction;

	EXPECT_FALSE(doTriangle(supports, direction));
	EXPECT_EQ(1, supports.size());
	EXPECT_EQ(mp0, supports[0]);
	EXPECT_TRUE(parallel(vec3(-1.0f, 0.0f, 0.0f), direction));
}

TEST(gjk, doTriangle_line_case_0)
{
	MinkowskiPoint mp0 (vec3(1.0f, -1.0f, 0.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(2.0f, -1.0f, 0.0f), 0, 2);
	MinkowskiPoint mp2 (vec3(1.5f, 1.0f, 0.0f), 0, 3);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2};

	vec3 direction;

	EXPECT_FALSE(doTriangle(supports, direction));
	EXPECT_EQ(2, supports.size());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp0)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp2)!=supports.end());
	//the direction vector should point left, outside of the triangle
	EXPECT_TRUE(sameDirection(direction, vec3(-1.0f, 0.0f, 0.0f)));
	//better: should point in the same direction as one of the edges coming from the remaining point
	EXPECT_TRUE(sameDirection(mp2 - mp1,direction ));
}

TEST(gjk, doTriangle_line_case_1)
{
	MinkowskiPoint mp0 (vec3(-2.0f, -1.0f, 0.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(-1.0f, -1.0f, 0.0f), 0, 2);
	MinkowskiPoint mp2 (vec3(-1.5f, 1.0f, 0.0f), 0, 3);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2};

	vec3 direction;

	EXPECT_FALSE(doTriangle(supports, direction));
	EXPECT_EQ(2, supports.size());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp1)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp2)!=supports.end());
	//the direction vector should point right, outside of the triangle
	EXPECT_TRUE(sameDirection(direction, vec3(1.0f, 0.0f, 0.0f)));

	//better: should point in the same direction as one of the edges coming from the remaining point
	EXPECT_TRUE(sameDirection(mp2- mp0, direction ));
}

TEST(gjk, doTetrahedron_enclosing)
{
	MinkowskiPoint mp0 (vec3(-1.0f, -1.0f, -1.0f), 0, 0);
	MinkowskiPoint mp1 (vec3(1.0f, -1.0f, -1.0f), 0, 1);
	MinkowskiPoint mp2 (vec3(0.0f, -1.0f, 1.0f), 0, 2);
	MinkowskiPoint mp3 (vec3(0.0f, 1.0f, 0.0f), 0, 3);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2, mp3};

	vec3 direction;

	EXPECT_TRUE(doTetrahedron(supports, direction));
	EXPECT_EQ(4, supports.size());
}

TEST(gjk, doTetrahedron_left_triangle)
{
	MinkowskiPoint mp0 (vec3(1.0f, -1.0f, -1.5f), 0, 0);

	MinkowskiPoint mp1 (vec3(3.0f, -1.0f, -1.5f), 0, 1);
	MinkowskiPoint mp2 (vec3(2.0f, -1.0f, 0.5f), 0, 2);
	MinkowskiPoint mp3 (vec3(2.0f, 1.0f, -0.5f), 0, 3);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2, mp3};

	vec3 direction;

	EXPECT_FALSE(doTetrahedron(supports, direction));
	EXPECT_EQ(3, supports.size());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp0)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp2)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp3)!=supports.end());

	EXPECT_TRUE(sameDirection(direction, vec3(-1.0f, 0.0f, 0.0f)));

	//vector shall point outside of the tetrahedron
	EXPECT_TRUE(sameDirection(direction, mp3 - mp1));
}

TEST(gjk, doTetrahedron_right_triangle)
{
	MinkowskiPoint mp0 (vec3(-3.0f, -1.0f, -1.5f), 0, 0);
	MinkowskiPoint mp1 (vec3(-1.0f, -1.0f, -1.5f), 0, 1);
	MinkowskiPoint mp2 (vec3(-2.0f, -1.0f, 0.5f), 0, 2);
	MinkowskiPoint mp3 (vec3(-2.0f, 1.0f, -0.5f), 0, 3);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2, mp3};

	vec3 direction;

	EXPECT_FALSE(doTetrahedron(supports, direction));
	EXPECT_EQ(3, supports.size());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp1)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp2)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp3)!=supports.end());

	EXPECT_TRUE(sameDirection(direction, vec3(1.0f, 0.0f, 0.0f)));

	//vector shall point outside of the tetrahedron
	EXPECT_TRUE(sameDirection(direction, mp3 - mp0));
}


TEST(gjk, doTetrahedron_front_triangle)
{

	//carefull with the winding! counter clockwise
	MinkowskiPoint mp0 (vec3(1.0f, -1.0f, -1.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(-1.0f, -1.0f, -1.0f), 0, 0);
	MinkowskiPoint mp2 (vec3(0.0f, -1.0f, -2.0f), 0, 2);
	MinkowskiPoint mp3 (vec3(0.0f, 1.0f, -1.0f), 0, 3);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2, mp3};

	vec3 direction;

	EXPECT_FALSE(doTetrahedron(supports, direction));
	EXPECT_EQ(3, supports.size());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp3)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp1)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), mp0)!=supports.end());

	EXPECT_TRUE(sameDirection(direction, vec3(0.0f, 0.0f, 1.0f)));

	//vector shall point outside of the tetrahedron
	EXPECT_TRUE(sameDirection(direction, mp3 - mp2));
}

TEST(gjk, doTetrahedron_left_point)
{
	MinkowskiPoint mp0 (vec3(2.0f, 0.0f, 1.0f), 0, 1);
	MinkowskiPoint mp1 (vec3(3.0f, 0.0f, 0.0f), 0, 0);
	MinkowskiPoint mp2 (vec3(2.0f, 1.0f, 0.0f), 0, 2);
	MinkowskiPoint mp3 (vec3(1.0f, 0.0f, 0.0f), 0, 3);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2, mp3};

	vec3 direction;

	EXPECT_FALSE(doTetrahedron(supports, direction));
	EXPECT_EQ(1, supports.size());
	EXPECT_EQ(mp3, supports[0]);

	EXPECT_TRUE(parallel(direction, vec3(-1.0f, 0.0f, 0.0f)));
}

TEST(gjk, doDistance_point)
{
	MinkowskiPoint mp0 (vec3(3.4f, 0.0f, 0.0f), 0, 0);
	std::vector<MinkowskiPoint> supports = {mp0};

	EXPECT_FLOAT_EQ(3.4f, doDistance(supports));
}

TEST(gjk, doDistance_line_orthogonal)
{
	MinkowskiPoint mp0 (vec3(2.5f, 1.0f, 0.0f), 0, 0);
	MinkowskiPoint mp1 (vec3(2.5f, -1.0f, 0.0f), 0, 0);
	std::vector<MinkowskiPoint> supports = {mp0, mp1};

	EXPECT_FLOAT_EQ(2.5f, doDistance(supports));
}

TEST(gjk, doDistance_triangle_orthogonal)
{
	MinkowskiPoint mp0 (vec3(2.0f, -1.0f, -1.0f), 0, 0);
	MinkowskiPoint mp1 (vec3(2.0f, 1.0f, -1.0f), 0, 1);
	MinkowskiPoint mp2 (vec3(2.0f, 0.0f, 1.0f), 0, 2);

	std::vector<MinkowskiPoint> supports = {mp0, mp1, mp2};

	EXPECT_FLOAT_EQ(2.0f, doDistance(supports));
}

struct test_GJK_intersection : ::testing::Test
{
	vector<Vertex> vertices0;
	vector<Vertex> vertices1;

	vector<unsigned int> indices0;
	vector<unsigned int> indices1;

	vec3 relative_position;

	test_GJK_intersection()
	{
		vertices0 = {	
						Vertex(0.0f, 0.0f, 0.0f, 1.0f),
						Vertex(1.0f, 0.0f, 0.0f, 1.0f),
						Vertex(0.5f, 1.0f, 0.5f, 1.0f),
						Vertex(0.5f, 0.0f, 1.0f, 1.0f)
					};

		vertices1 = {
						Vertex(0.0f, 0.0f, 0.0f, 1.0f),
						Vertex(1.0f, 0.0f, 0.0f, 1.0f),
						Vertex(0.5f, 1.0f, 0.5f, 1.0f),
						Vertex(0.5f, 0.0f, 1.0f, 1.0f)
					};

		indices0 = {0,1,2,3};
		indices1 = {0,1,2,3};
	}

	virtual ~test_GJK_intersection()
	{}
};

TEST_F(test_GJK_intersection, in_volume)
{
	relative_position = vec3(0.0f, 0.5f, 0.0f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);
	
	EXPECT_TRUE(intersection(mg.begin(), mg.end()));
}


TEST_F(test_GJK_intersection, none)
{
	relative_position = vec3(0.0f, 2.0f, 0.0f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);
	
	EXPECT_FALSE(intersection(mg.begin(), mg.end()));
}

TEST_F(test_GJK_intersection, on_vertex)
{
	relative_position = vec3(0.0f, 1.0f, 0.0f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);
	
	EXPECT_TRUE(intersection(mg.begin(), mg.end()));
}

TEST_F(test_GJK_intersection, vertex_vertex)
{
	//This test also tests the capability of edge-cases to determine exits properly
	relative_position = vec3(-1.0f, 0.0f, 0.0f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);
	
	EXPECT_TRUE(intersection(mg.begin(), mg.end()));
}

struct test_GJK_distance : ::testing::Test
{
	vector<Vertex> vertices0;
	vector<Vertex> vertices1;

	vector<unsigned int> indices0;
	vector<unsigned int> indices1;

	vec3 relative_position;

	test_GJK_distance()
	{
		vertices0 = {	
						Vertex(0.0f, 0.0f, 0.0f, 1.0f),
						Vertex(1.0f, 0.0f, 0.0f, 1.0f),
						Vertex(0.5f, 1.0f, 0.5f, 1.0f),
						Vertex(0.5f, 0.0f, 1.0f, 1.0f)
					};

		vertices1 = {
						Vertex(0.0f, 0.0f, 0.0f, 1.0f),
					};

		indices0 = {0,1,2,3};
		indices1 = {0};
	}

	~test_GJK_distance()
	{}
};

TEST_F(test_GJK_distance, intersection)
{
	relative_position = vec3(0.0f, 0.0f, 0.0f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);
	
	EXPECT_EQ(0.0f, gjk::distance(mg.begin(), mg.end()));
}

TEST_F(test_GJK_distance, left_to_vertex)
{
	relative_position = vec3(-1.0f, 0.0f, 0.0f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);

	vector<MinkowskiPoint> supports;

	EXPECT_EQ(1.0f, gjk::distance(mg.begin(), mg.end(), &supports));

	ASSERT_EQ(1, supports.size());
	EXPECT_EQ(0, supports[0].i0);
	EXPECT_EQ(0, supports[0].i1);
}

TEST_F(test_GJK_distance, top_to_vertex)
{
	relative_position = vec3(0.5f, 2.0f, 0.5f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);

	vector<MinkowskiPoint> supports;

	EXPECT_FALSE(intersection(mg.begin(), mg.end()));
	EXPECT_EQ(1.0f, gjk::distance(mg.begin(), mg.end(), &supports));

	ASSERT_EQ(1, supports.size());
	EXPECT_EQ(2, supports[0].i0);
	EXPECT_EQ(0, supports[0].i1);
}

TEST_F(test_GJK_distance, left_to_edge)
{
	relative_position = vec3(-1.0f, 1.0f, 0.0f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);

	vector<MinkowskiPoint> supports;

	float d = gjk::distance(mg.begin(), mg.end(), &supports);
	EXPECT_LT(1.0f, d);
	EXPECT_GT(3.0f, d);

	ASSERT_EQ(2, supports.size());
	//the position is not correct in the expected value,
	// it doesn't need to be
	//  for the comparison only the indices are important
	MinkowskiPoint expected0(vec3(0.0f), 0,0);
	MinkowskiPoint expected1(vec3(0.0f), 2,0);
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), expected0)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), expected1)!=supports.end());
}

TEST_F(test_GJK_distance, under_face)
{
	relative_position = vec3(0.5f, -1.0f, 0.5f);	

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end(),
			relative_position
			);

	vector<MinkowskiPoint> supports;

	float d = gjk::distance(mg.begin(), mg.end(), &supports);
	EXPECT_FLOAT_EQ(1.0f, d);

	ASSERT_EQ(3, supports.size());
	//the position is not correct in the expected value,
	// it doesn't need to be
	//  for the comparison only the indices are important
	MinkowskiPoint expected0(vec3(0.0f), 0,0);
	MinkowskiPoint expected1(vec3(0.0f), 1,0);
	MinkowskiPoint expected2(vec3(0.0f), 3,0);
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), expected0)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), expected1)!=supports.end());
	EXPECT_TRUE(std::find(supports.begin(), supports.end(), expected2)!=supports.end()); }


int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
