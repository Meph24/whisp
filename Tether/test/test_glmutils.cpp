#include "../src/glmutils.hpp"

#include <gtest/gtest.h>

#include <string>
#include <sstream>

using glm::vec3;
using glm::vec4;
using glm::mat4;

using std::stringstream;
using std::string;

TEST(test_glmutils, sqlen)
{
	vec3 v(1, 2, 3);
	EXPECT_EQ(v.x*v.x + v.y*v.y + v.z*v.z, glm::sqlen(v));
}

TEST(test_glmutils, m4v3_mult)
{
	//is considered the same as m4v4 mult with v.w = 1
	vec3 v3(1,2,3);
	vec4 v4(v3, 1);
	mat4 m(1);

	vec3 ret = m*v3;
	vec4 expect = m*v4; //implemented by library
	EXPECT_EQ(expect.x, ret.x);
	EXPECT_EQ(expect.y, ret.y);
	EXPECT_EQ(expect.z, ret.z);
}

TEST(test_glmutils, v3m4_mult)
{
	vec3 v3(1,2,3);
	vec4 v4(v3, 1);
	mat4 m(	1,2,3,4,
			5,6,7,8,
			1,4,5,6,
			1,1,1,1
			);

	/* TODO reevaluate
	 *	refactor if necessary
	 *	both assumptions cannot be true
	 */
	bool assumption0 = glm::vecEquals(m*v3, v3*m);
	bool assumption1 = glm::vecEquals(v3*m, vec3(v4*m));
	ASSERT_TRUE(assumption0) 
		<< "ASSUMPTION0 : glm::vecEquals((m*v3), (v3*m)) : " 
			<< assumption0;
	ASSERT_FALSE(assumption1)
		<< "ASSUMPTION1 : glm::vecEquals(v3*m, vec3(v4*m)) : " 
			<< assumption1;
	//  ASSUMPTION0 as practical tranformation order insensitive
	//	ASSUMPTION1 mathematical/ consistent with glm implementation
}

TEST(test_glmutils, v4v3_v3v4_add)
{
	vec3 v3 (1, 2, 3);
	vec4 other (2,3,4,5);
	EXPECT_EQ(v3 + other, other + v3);

	EXPECT_EQ(5, (other + v3).w);
}

TEST(test_glmutils, ostream)
{
	vec3 v(2,3,4);
	stringstream ss;

	ss << v;

	EXPECT_EQ("vec3(2.000000, 3.000000, 4.000000)", ss.str());
}

TEST(test_glmutils, rotateDeg)
{
	vec3 v(1,0,0);

	mat4 rDeg = glm::rotateDeg(90.0f, vec3(0,0,1));
	
	EXPECT_TRUE(glm::vecEquals(vec3(0,1,0), rDeg * v));
}



TEST(test_glmutils, direction_to_rotation_angle)
{
	{
		vec3 v(0.0f, 0.0f, 1.0f);
		EXPECT_TRUE(glm::vecEquals(directionToAngleRotation(v), vec3(0.0f, 0.0f, 0.0f)));	
	}
	{
		//exactly 180° results in x being turned
			vec3 v(0.0f, 0.0f, -1.0f);
		EXPECT_TRUE(glm::vecEquals(directionToAngleRotation(v), vec3(TAU/2, 0.0f, 0.0f)));	
	}
	{
		//dumb input
		vec3 v(0.0f, 0.0f, 0.0f);
		EXPECT_TRUE(glm::vecEquals(directionToAngleRotation(v), vec3(0.0f, 0.0f, 0.0f)));	
	}
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
