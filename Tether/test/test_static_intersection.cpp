#include "../src/static_intersection.hpp"

#include "../src/glmutils.hpp"

#include <gtest/gtest.h>

struct test_static_intersection : ::testing::Test
{
	vec3 pr;
	vec3 p1;
	vec3 p2;
	vec3 pn;
	test_static_intersection()
		: pr(0,0,0)
		, p1(1,0,0)
		, p2(0,1,0)
		, pn(glm::cross(p1, p2))
	{}


};

TEST_F(test_static_intersection, rayIntersectsPlane_orthogonal_ray_plane_normal_signature)
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


TEST_F(test_static_intersection, rayIntersectsPlane_orthogonal_ray_plane_2vector_signature)
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

TEST_F(test_static_intersection, rayIntersectsPlane_parallel_ray_plane_normal_signature)
{
	vec3 rr(0,0,-1);
	vec3 rv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, rv));

	bool ret = rayIntersectsPlane(pr, pn, rr, rv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, rayIntersectsPlane_parallel_ray_plane_2vector_signature)
{
	vec3 rr(0,0,-1);
	vec3 rv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, rv));

	bool ret = rayIntersectsPlane(pr, p1, p2, rr, rv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, rayIntersectsPlane_non_parallel_ray_plane_normal_signature)
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


TEST_F(test_static_intersection, rayIntersectsPlane_non_parallel_ray_plane_2vector_signature)
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

TEST_F(test_static_intersection, rayIntersectsPlane_contained_ray_plane_normal_signature)
{
	vec3 rr(0,1,0);
	vec3 rv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f , glm::dot(rv, pn));
	ASSERT_EQ(pr+2*p2, rr+rv);
	ASSERT_EQ(pr+p2, rr);

	bool ret = rayIntersectsPlane(pr, pn, rr, rv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, rayIntersectsPlane_contained_ray_plane_2vector_signature)
{
	vec3 rr(0,1,0);
	vec3 rv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f , glm::dot(rv, pn));
	ASSERT_EQ(pr+2*p2, rr+rv);
	ASSERT_EQ(pr+p2, rr);

	bool ret = rayIntersectsPlane(pr, p1, p2, rr, rv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, lineIntersectsPlane_orthogonal_line_plane_normal_signature)
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


TEST_F(test_static_intersection, lineIntersectsPlane_orthogonal_line_plane_2vector_signature)
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

TEST_F(test_static_intersection, lineIntersectsPlane_parallel_line_plane_normal_signature)
{
	vec3 lr(0,0,-1);
	vec3 lv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, lv));

	bool ret = lineIntersectsPlane(pr, pn, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, lineIntersectsPlane_parallel_line_plane_2vector_signature)
{
	vec3 lr(0,0,-1);
	vec3 lv = p1;

	ASSERT_EQ(0.0f, glm::dot(pn, lv));

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, lineIntersectsPlane_non_parallel_line_plane_normal_signature)
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


TEST_F(test_static_intersection, lineIntersectsPlane_non_parallel_line_plane_2vector_signature)
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

TEST_F(test_static_intersection, lineIntersectsPlane_contained_line_plane_normal_signature)
{
	vec3 lr(0,1,0);
	vec3 lv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f, glm::dot(pn, lv));
	ASSERT_EQ(pr+2*p2, lr+lv);
	ASSERT_EQ(pr+p2, lr);

	bool ret = lineIntersectsPlane(pr, pn, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, lineIntersectsPlane_contained_line_plane_2vector_signature)
{
	vec3 lr(0,1,0);
	vec3 lv(0,1,0);

	ASSERT_FLOAT_EQ(0.0f, glm::dot(pn, lv));
	ASSERT_EQ(pr+2*p2, lr+lv);
	ASSERT_EQ(pr+p2, lr);

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, lineIntersectsPlane_out_of_reach_line_plane_normal_signature)
{
	vec3 lr(0,0,-2);
	vec3 lv(0,0,1);

	ASSERT_EQ(lv, glm::normalize(pn));

	bool ret = lineIntersectsPlane(pr, pn, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, lineIntersectsPlane_out_of_reach_line_plane_2vector_signature)
{
	vec3 lr(0,0,-2);
	vec3 lv(0,0,1);

	ASSERT_EQ(lv, glm::normalize(pn));

	bool ret = lineIntersectsPlane(pr, p1, p2, lr, lv);

	EXPECT_FALSE(ret);
}

TEST_F(test_static_intersection, pointPartOfPlane_root)
{
	vec3 p(0,0,0);
	
	vec2 out_coeff;
	EXPECT_TRUE(pointPartOfPlane(pr, p1, p2, p, &out_coeff));

	vec2 expect(p.x,p.y);
	EXPECT_EQ(expect, out_coeff) << "Expect : " << glm::to_string(expect) << "\nActual : " << glm::to_string(out_coeff) << '\n';
}

TEST_F(test_static_intersection, pointPartOfPlane_in_plane_non_root)
{
	vec3 p(2,3,0);
	
	vec2 out_coeff;
	EXPECT_TRUE(pointPartOfPlane(pr, p1, p2, p, &out_coeff));

	vec2 expect(p.x,p.y);
	EXPECT_EQ(expect, out_coeff) << "Expect : " << glm::to_string(expect) << "\nActual : " << glm::to_string(out_coeff) << '\n';
}

TEST_F(test_static_intersection, pointPartOfPlane_under)
{
	vec3 p(2,3,1);
	EXPECT_FALSE(pointPartOfPlane(pr, p1, p2, p));
}

TEST_F(test_static_intersection, pointPartOfPlane_normal_signature_root)
{
	vec3 p(0,0,0);
	EXPECT_TRUE(pointPartOfPlane(pr, pn, p));
}

TEST_F(test_static_intersection, pointPartOfPlane_normal_signature_in_plane_non_root)
{
	vec3 p(2,3,0);
	EXPECT_TRUE(pointPartOfPlane(pr, pn, p));
}

TEST_F(test_static_intersection, pointPartOfPlane_normal_signature_under)
{
	vec3 p(2,3,1);
	EXPECT_FALSE(pointPartOfPlane(pr, pn, p));
}

TEST_F(test_static_intersection, pointPartOfTriangle_root)
{
	vec3 p(0,0,0);
	EXPECT_TRUE(pointPartOfTriangle(pr, p1, p2, p));
}
TEST_F(test_static_intersection, pointPartOfTriangle_inside)
{
	vec3 p (0.4, 0.3, 0.0);
	EXPECT_TRUE(pointPartOfTriangle(pr, p1, p2, p));
}
TEST_F(test_static_intersection, pointPartOfTriangle_under)
{
	vec3 p (0.4, 0.3, -0.5);
	EXPECT_FALSE(pointPartOfTriangle(pr, p1, p2, p));
}
TEST_F(test_static_intersection, pointPartOfTriangle_nextto)
{
	vec3 p (0.8, 0.7, 0.0);
	EXPECT_FALSE(pointPartOfTriangle(pr, p1, p2, p));

	p = vec3(-0.1, 0.0, 0.0);
	EXPECT_FALSE(pointPartOfTriangle(pr, p1, p2, p));
}

TEST_F(test_static_intersection, pointPartOfParallelogram_root)
{
	vec3 p(0,0,0);
	EXPECT_TRUE(pointPartOfParallelogram(pr, p1, p2, p));
}
TEST_F(test_static_intersection, pointPartOfParallelogram_inside)
{
	vec3 p (0.4, 0.3, 0.0);
	EXPECT_TRUE(pointPartOfParallelogram(pr, p1, p2, p));

	p = vec3(0.8, 0.7, 0.0);
	EXPECT_TRUE(pointPartOfParallelogram(pr, p1, p2, p));
}
TEST_F(test_static_intersection, pointPartOfParallelogram_under)
{
	vec3 p (0.4, 0.3, -0.5);
	EXPECT_FALSE(pointPartOfParallelogram(pr, p1, p2, p));
}
TEST_F(test_static_intersection, pointPartOfParallelogram_nextto)
{
	vec3 p (1.0, 1.1, 0.0);
	EXPECT_FALSE(pointPartOfParallelogram(pr, p1, p2, p));

	p = vec3(-0.1, 0.0, 0.0);
	EXPECT_FALSE(pointPartOfParallelogram(pr, p1, p2, p));
}

TEST_F(test_static_intersection, rayIntersectsTriangle_root)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 rr(0,0,-1);
	vec3 rv(0,0,1);

	EXPECT_TRUE(rayIntersectsTriangle(pr, p1, p2, rr, rv, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0,0,0), out_pos);
	EXPECT_EQ(vec3(0,0,1), out_coeff);
}
TEST_F(test_static_intersection, rayIntersectsTriangle_inside)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 rr(0.4,0.3,-1);
	vec3 rv(0,0,1);

	EXPECT_TRUE(rayIntersectsTriangle(pr, p1, p2, rr, rv, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0.4,0.3,0), out_pos);
	EXPECT_EQ(vec3(0.4,0.3,1), out_coeff);
}
TEST_F(test_static_intersection, rayIntersectsTriangle_miss)
{
	vec3 rr1(0.7,0.8,-1);
	vec3 rv1(0,0,1);

	EXPECT_FALSE(rayIntersectsTriangle(pr, p1, p2, rr1, rv1));

	vec3 rr2(0,-0.1,-1);
	vec3 rv2(0,0,1);

	EXPECT_FALSE(rayIntersectsTriangle(pr, p1, p2, rr2, rv2));
}

TEST_F(test_static_intersection, rayIntersectsParallelogram_root)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 rr(0,0,-1);
	vec3 rv(0,0,1);

	EXPECT_TRUE(rayIntersectsParallelogram(pr, p1, p2, rr, rv, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0,0,0), out_pos);
	EXPECT_EQ(vec3(0,0,1), out_coeff);
}
TEST_F(test_static_intersection, rayIntersectsParallelogram_inside)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 rr1(0.4,0.3,-1);
	vec3 rv1(0,0,1);

	EXPECT_TRUE(rayIntersectsParallelogram(pr, p1, p2, rr1, rv1, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0.4,0.3,0), out_pos);
	EXPECT_EQ(vec3(0.4,0.3,1), out_coeff);
	
	vec3 rr2(0.7,0.8,-1);
	vec3 rv2(0,0,1);

	EXPECT_TRUE(rayIntersectsParallelogram(pr, p1, p2, rr2, rv2, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0.7,0.8,0), out_pos);
	EXPECT_EQ(vec3(0.7,0.8,1), out_coeff);

}
TEST_F(test_static_intersection, rayIntersectsParallelogram_miss)
{
	vec3 rr1(1.1,1.0,-1);
	vec3 rv1(0,0,1);

	EXPECT_FALSE(rayIntersectsParallelogram(pr, p1, p2, rr1, rv1));

	vec3 rr2(0,-0.1,-1);
	vec3 rv2(0,0,1);

	EXPECT_FALSE(rayIntersectsParallelogram(pr, p1, p2, rr2, rv2));
}



TEST_F(test_static_intersection, lineIntersectsTriangle_root)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 lr(0,0,-1);
	vec3 lv(0,0,1);

	EXPECT_TRUE(lineIntersectsTriangle(pr, p1, p2, lr, lv, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0,0,0), out_pos);
	EXPECT_EQ(vec3(0,0,1), out_coeff);
}
TEST_F(test_static_intersection, lineIntersectsTriangle_inside)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 lr(0.4,0.3,-1);
	vec3 lv(0,0,1);

	EXPECT_TRUE(lineIntersectsTriangle(pr, p1, p2, lr, lv, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0.4,0.3,0), out_pos);
	EXPECT_EQ(vec3(0.4,0.3,1), out_coeff);
}
TEST_F(test_static_intersection, lineIntersectsTriangle_miss)
{
	vec3 lr1(0.7,0.8,-1);
	vec3 lv1(0,0,1);

	EXPECT_FALSE(lineIntersectsTriangle(pr, p1, p2, lr1, lv1));

	vec3 lr2(0,-0.1,-1);
	vec3 lv2(0,0,1);

	EXPECT_FALSE(lineIntersectsTriangle(pr, p1, p2, lr2, lv2));
}

TEST_F(test_static_intersection, lineIntersectsTriangle_out_of_reach_miss)
{
	vec3 lr1(0.4,0.3,-2);
	vec3 lv1(0,0,1);

	EXPECT_FALSE(lineIntersectsTriangle(pr, p1, p2, lr1, lv1));
}

TEST_F(test_static_intersection, lineIntersectsParallelogram_root)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 lr(0,0,-1);
	vec3 lv(0,0,1);

	EXPECT_TRUE(lineIntersectsParallelogram(pr, p1, p2, lr, lv, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0,0,0), out_pos);
	EXPECT_EQ(vec3(0,0,1), out_coeff);
}
TEST_F(test_static_intersection, lineIntersectsParallelogram_inside)
{
	vec3 out_pos;
	vec3 out_coeff;

	vec3 lr1(0.4,0.3,-1);
	vec3 lv1(0,0,1);

	EXPECT_TRUE(lineIntersectsParallelogram(pr, p1, p2, lr1, lv1, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0.4,0.3,0), out_pos);
	EXPECT_EQ(vec3(0.4,0.3,1), out_coeff);
	
	vec3 lr2(0.7,0.8,-1);
	vec3 lv2(0,0,1);

	EXPECT_TRUE(lineIntersectsParallelogram(pr, p1, p2, lr2, lv2, &out_pos, &out_coeff));
	EXPECT_EQ(vec3(0.7,0.8,0), out_pos);
	EXPECT_EQ(vec3(0.7,0.8,1), out_coeff);

}
TEST_F(test_static_intersection, lineIntersectsParallelogram_miss)
{
	vec3 lr1(1.1,1.0,-1);
	vec3 lv1(0,0,1);

	EXPECT_FALSE(lineIntersectsParallelogram(pr, p1, p2, lr1, lv1));

	vec3 lr2(0,-0.1,-1);
	vec3 lv2(0,0,1);

	EXPECT_FALSE(lineIntersectsParallelogram(pr, p1, p2, lr2, lv2));
}
TEST_F(test_static_intersection, lineIntersectsParallelogram_out_of_reach_miss)
{
	vec3 lr1(0.4,0.3,-2);
	vec3 lv1(0,0,1);

	EXPECT_FALSE(lineIntersectsParallelogram(pr, p1, p2, lr1, lv1));
	
	vec3 lr2(0.7,0.8,-2);
	vec3 lv2(0,0,1);

	EXPECT_FALSE(lineIntersectsParallelogram(pr, p1, p2, lr2, lv2));
}



int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
