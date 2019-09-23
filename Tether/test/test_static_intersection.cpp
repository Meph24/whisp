#include "../src/static_intersection.hpp"

#include "../src/glmutils.hpp"

#include <gtest/gtest.h>

struct test_static_intersection_rayIntersectsPlane : ::testing::Test
{
	vec3 pr;
	vec3 p1;
	vec3 p2;
	vec3 pn;
	test_static_intersection_rayIntersectsPlane()
		: pr(0,0,0)
		, p1(1,0,0)
		, p2(0,1,0)
		, pn(glm::cross(p1, p2))
	{}


};

TEST_F(test_static_intersection_rayIntersectsPlane , orthogonal_ray_plane_normal_signature)
{
	vec3 rr(0,1,-1);
	vec3 rv (0,0,1);

	ASSERT_EQ(rv, glm::normalize(pn));

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, pn, rr, rv, &out_pos, &out_ray_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(0,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_ray_coeff);
}


TEST_F(test_static_intersection_rayIntersectsPlane , orthogonal_ray_plane_2vector_signature)
{
	vec3 rr(0,1,-1);
	vec3 rv (0,0,1);

	ASSERT_EQ(rv, glm::normalize(pn));

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, p1, p2, rr, rv, &out_pos, &out_ray_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(0,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_ray_coeff);
}

TEST_F(test_static_intersection_rayIntersectsPlane , parallel_ray_plane_normal_signature)
{
	vec3 rr(0,0,-1);
	vec3 rv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, rv));

	bool ret = rayIntersectsPlane(pr, pn, rr, rv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_rayIntersectsPlane , parallel_ray_plane_2vector_signature)
{
	vec3 rr(0,0,-1);
	vec3 rv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, rv));

	bool ret = rayIntersectsPlane(pr, p1, p2, rr, rv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_rayIntersectsPlane , non_parallel_ray_plane_normal_signature)
{
	vec3 rr(0,0,-1);
	vec3 rv(1,1,1);

	ASSERT_FALSE((fabs(glm::dot(pn, rv)) < 0.00001f)) << "Expected Not-Near: " << 0.0f << "\nActual : " << glm::dot(pn, rv) << '\n';

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, pn, rr, rv, &out_pos, &out_ray_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(1,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_ray_coeff);
}


TEST_F(test_static_intersection_rayIntersectsPlane , non_parallel_ray_plane_2vector_signature)
{
	vec3 rr(0,0,-1);
	vec3 rv(1,1,1);

	ASSERT_FALSE((fabs(glm::dot(pn, rv)) < 0.00001f)) << "Expected Not-Near: " << 0.0f << "\nActual : " << glm::dot(pn, rv) << '\n';

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, p1, p2, rr, rv, &out_pos, &out_ray_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(1,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_ray_coeff);
}

TEST_F(test_static_intersection_rayIntersectsPlane , contained_ray_plane_normal_signature)
{
	vec3 rr(0,1,0);
	vec3 rv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f , glm::dot(rv, pn));
	ASSERT_EQ(pr+2*p2, rr+rv);
	ASSERT_EQ(pr+p2, rr);

	bool ret = rayIntersectsPlane(pr, pn, rr, rv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_rayIntersectsPlane , contained_ray_plane_2vector_signature)
{
	vec3 rr(0,1,0);
	vec3 rv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f , glm::dot(rv, pn));
	ASSERT_EQ(pr+2*p2, rr+rv);
	ASSERT_EQ(pr+p2, rr);

	bool ret = rayIntersectsPlane(pr, p1, p2, rr, rv);

	EXPECT_FALSE(ret);
}


struct test_static_intersection_lineIntersectsPlane : ::testing::Test
{
	vec3 pr;
	vec3 p1;
	vec3 p2;
	vec3 pn;
	test_static_intersection_lineIntersectsPlane()
		: pr(0,0,0)
		, p1(1,0,0)
		, p2(0,1,0)
		, pn(glm::cross(p1, p2))
	{}


};

TEST_F(test_static_intersection_lineIntersectsPlane , orthogonal_line_plane_normal_signature)
{
	vec3 lr(0,1,-1);
	vec3 lv (0,0,1);

	ASSERT_EQ(lv, glm::normalize(pn));

	vec3 out_pos;
	float out_line_coeff;

	bool ret = lineIntersectsPlane(pr, pn, lr, lv, &out_pos, &out_line_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(0,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_line_coeff);
}


TEST_F(test_static_intersection_lineIntersectsPlane , orthogonal_line_plane_2vector_signature)
{
	vec3 lr(0,1,-1);
	vec3 lv (0,0,1);

	ASSERT_EQ(lv, glm::normalize(pn));

	vec3 out_pos;
	float out_line_coeff;

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv, &out_pos, &out_line_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(0,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_line_coeff);
}

TEST_F(test_static_intersection_lineIntersectsPlane , parallel_line_plane_normal_signature)
{
	vec3 lr(0,0,-1);
	vec3 lv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, lv));

	bool ret = lineIntersectsPlane(pr, pn, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_lineIntersectsPlane , parallel_line_plane_2vector_signature)
{
	vec3 lr(0,0,-1);
	vec3 lv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, lv));

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_lineIntersectsPlane , non_parallel_line_plane_normal_signature)
{
	vec3 lr(0,0,-1);
	vec3 lv(1,1,1);

	ASSERT_NE(0.0f, glm::dot(pn, lv));

	vec3 out_pos;
	float out_line_coeff;

	bool ret = lineIntersectsPlane(pr, pn, lr, lv, &out_pos, &out_line_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(1,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_line_coeff);
}


TEST_F(test_static_intersection_lineIntersectsPlane , non_parallel_line_plane_2vector_signature)
{
	vec3 lr(0,0,-1);
	vec3 lv(1,1,1);

	ASSERT_NE(0.0f, glm::dot(pn, lv));

	vec3 out_pos;
	float out_line_coeff;

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv, &out_pos, &out_line_coeff);

	EXPECT_TRUE(ret);
	EXPECT_EQ(vec3(1,1,0), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	EXPECT_FLOAT_EQ(1.0f, out_line_coeff);
}

TEST_F(test_static_intersection_lineIntersectsPlane , contained_line_plane_normal_signature)
{
	vec3 lr(0,1,0);
	vec3 lv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f, glm::dot(pn, lv));
	ASSERT_EQ(pr+2*p2, lr+lv);
	ASSERT_EQ(pr+p2, lr);

	bool ret = lineIntersectsPlane(pr, pn, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_lineIntersectsPlane , contained_line_plane_2vector_signature)
{
	vec3 lr(0,1,0);
	vec3 lv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f, glm::dot(pn, lv));
	ASSERT_EQ(pr+2*p2, lr+lv);
	ASSERT_EQ(pr+p2, lr);

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_lineIntersectsPlane , out_of_reach_line_plane_normal_signature)
{
	vec3 lr(0,0,-2);
	vec3 lv(0,0,1);

	ASSERT_EQ(lv, glm::normalize(pn));

	bool ret = lineIntersectsPlane(pr, pn, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection_lineIntersectsPlane , out_of_reach_line_plane_2vector_signature)
{
	vec3 lr(0,0,-2);
	vec3 lv(0,0,1);

	ASSERT_EQ(lv, glm::normalize(pn));

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv);

	EXPECT_FALSE(ret);
}


int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
