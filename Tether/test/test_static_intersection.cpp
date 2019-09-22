#include "../src/static_intersection.hpp"

#include "../src/glmutils.hpp"

#include <gtest/gtest.h>

struct test_static_intersection_rayIntersectsPlane : ::testing::Test
{
	vec3 pr;
	vec3 p1;
	vec3 p2;
	vec3 pn;
	test_static_intersection()
		: pr(1,1,1)
		, p1(1,0,0)
		, p2(0,1,0)
		, pn(glm::cross(p1, p2))
	{}


};

TEST_F(test_static_intersection_rayIntersectsPlane , orthogonal_ray-plane_normal_signature)
{
	vec3 orthogonal_ray_root(0,0,-1);
	vec3 orthogonal_ray_vec (0,0,1);

	ASSERT_EQ(orthogonal_ray_vec, glm::normalize(pn));

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, pn, orthogonal_ray_root, orthogonal_ray_vec, &out_pos, &out_ray_coeff);

	ASSERT_TRUE(ret);
	ASSERT_EQ(vec3(0,0,1), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	ASSERT_EQ(1.0f, out_ray_coeff);
}


TEST_F(test_static_intersection_rayIntersectsPlane , orthogonal_ray-plane_2vector_signature)
{
	vec3 orthogonal_ray_root(0,0,-1);
	vec3 orthogonal_ray_vec (0,0,1);

	ASSERT_EQ(orthogonal_ray_vec, glm::normalize(pn));

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, p1, p2, orthogonal_ray_root, orthogonal_ray_vec, &out_pos, &out_ray_coeff);

	ASSERT_TRUE(ret);
	ASSERT_EQ(vec3(0,0,1), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	ASSERT_EQ(1.0f, out_ray_coeff);
}

TEST_F(test_static_intersection_rayIntersectsPlane , parallel_ray-plane_normal_signature)
{
	vec3 parallel_ray_root(0,0,0);
	vec3 parallel_ray_vec = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, parallel_ray_vec));

	bool ret = rayIntersectsPlane(pr, pn, orthogonal_ray_root, orthogonal_ray_vec);

	ASSERT_FALSE(ret);
}

TEST_F(test_static_intersection_rayIntersectsPlane , parallel_ray-plane_2vector_signature)
{
	vec3 parallel_ray_root(0,0,0);
	vec3 parallel_ray_vec = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, parallel_ray_vec));

	bool ret = rayIntersectsPlane(pr, p1, p2, orthogonal_ray_root, orthogonal_ray_vec);

	ASSERT_FALSE(ret);
}

TEST_F(test_static_intersection_rayIntersectsPlane , non_parallel_ray-plane_normal_signature)
{
	vec non_parallel_ray_root(0,0,0);
	vec non_parallel_ray_vec(1,1,1);

	ASSERT_NE(0.0f, glm::dot(pn, parralel_ray_vec));

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, pn, orthogonal_ray_root, orthogonal_ray_vec, &out_pos, &out_ray_coeff);

	ASSERT_TRUE(ret);
	ASSERT_EQ(vec3(1,1,1), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	ASSERT_EQ(1.0f, out_ray_coeff);
}


TEST_F(test_static_intersection_rayIntersectsPlane , non_parallel_ray-plane_normal_signature)
{
	vec non_parallel_ray_root(0,0,0);
	vec non_parallel_ray_vec(1,1,1);

	ASSERT_NE(0.0f, glm::dot(pn, parralel_ray_vec));

	vec3 out_pos;
	float out_ray_coeff;

	bool ret = rayIntersectsPlane(pr, p1, p2, orthogonal_ray_root, orthogonal_ray_vec, &out_pos, &out_ray_coeff);

	ASSERT_TRUE(ret);
	ASSERT_EQ(vec3(1,1,1), out_pos);
	//intersection at rv tip (at rr+rv*1.0)
	ASSERT_EQ(1.0f, out_ray_coeff);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
