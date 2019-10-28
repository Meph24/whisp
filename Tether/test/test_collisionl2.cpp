#include "../src/collisionl2.hpp"

#include <gtest/gtest.h>

using namespace collisionl2;

/*
 *	We create triangles which we will collide
 *		by spawning one at 0,0,0 and the other at 0,0,-1
 *		moving the latter with a velocity of 0,0,2
 * Test Cases to explore:
 *	*	Edge-Edge only collision.
 *	*	Vertex-Face only collision. (which is the one where only 1 of the 2)
 *			steps of vertex face detection detects the collision
 *	*	Vertex-Edge collision to test the spacial boundaries
 *			will result in multiple edge-edge and one vertex-face collision
 *	*	Feature precedence
 *		-	Edge-Edge only with sloped edge
 *		-	Vertex only with one vertex slightly forward
 *	*	Test relativity correctness
 *			by creating a case where both triangles move
 * */

struct test_collisionl2_linear_interpolation : public ::testing::Test
{
	Mesh mesh0, mesh1, mesh2,
		mesh1s, mesh2s;		// "sloped"
	Mesh meshp; // single vertex
public:
	test_collisionl2_linear_interpolation()
	{
		unsigned int indices[3] = {0,1,2};
		vec3 vertices[3];


		vertices[0] = vec3(0,0,0);
		vertices[1] = vec3(1,0,0);
		vertices[2] = vec3(0,1,0);
		mesh0 = Mesh(vertices, vertices+3, indices, indices+3);

		meshp = Mesh(vertices, vertices+1, indices, indices);	//extract only the single point at 0,0,0, which's vector is kongruent with the pos_vector

		//this triangle is made so, that no vertex of m0 and m1 lies inside
		//	of the respective other triangle, but the edges collide
		vertices[0] = vec3(-1,0.5,0);
		vertices[1] = vec3(1,0.5,0);
		vertices[2] = vec3(1,1,0);
		mesh1 = Mesh(vertices, vertices+3, indices, indices+3);

		//this triangle is made so, that it lies completely inside m0
		//	while not even touching and edge of it
		vertices[0] = vec3(0.1,0.1,0);
		vertices[1] = vec3(0.1,0.5,0);
		vertices[2] = vec3(0.5,0.1,0);
		mesh2 = Mesh(vertices, vertices+3, indices, indices+3);

		//on the sloped triangles we put one vertex more forward in z direction
		//	making it sloped to the z plane
		//	these triangles can be used for testing collision time,
		//	as they are to hit first
		//	could be done with rotation, too, but this way the result is
		//	easier to precalculate for testing
		mesh1s = mesh1;
		mesh1s.vertices[0].z = 0.5;
		mesh2s = mesh2;
		mesh2s.vertices[0].z = 0.5; 
	}

	~test_collisionl2_linear_interpolation()
	{
	
	}
};

TEST_F(test_collisionl2_linear_interpolation, point_face_collision)
{
	Model p (&meshp);
	Model m (&mesh0);	
	vec3 p_pos (0,0,-1);
	vec3 m_pos (0,0,0);
	vec3 p_v (0,0,2);
	vec3 m_v (0,0,0);

	float t0, t1;
	t0 = 0.0;
	t1 = 1.0;

	vector<SubmodelCollision> colls = linearInterpolation(
			t0, t1,
			m, p,
			m_pos, p_pos,
			m_v, p_v
			);

	ASSERT_EQ(1, colls.size());

	SubmodelCollision sc = colls[0];

	EXPECT_EQ(0.5, sc.time);
	EXPECT_EQ(vec3(0,0,0), sc.pos);

	EXPECT_EQ(CollisionFeature::TYPE::Face, sc.feat0.type);
	EXPECT_EQ(CollisionFeature::TYPE::Vertex, sc.feat1.type);

	EXPECT_EQ(0, sc.feat0.mesh_indices[0]);
	EXPECT_EQ(1, sc.feat0.mesh_indices[1]);
	EXPECT_EQ(2, sc.feat0.mesh_indices[2]);

	EXPECT_EQ(0, sc.feat1.mesh_indices[0]);
}

TEST_F(test_collisionl2_linear_interpolation, edge_edge_only)
{
	Model m0(&mesh0);
	Model m1(&mesh1);
	vec3 m0_pos (0,0,0);
	vec3 m1_pos (0,0,-1);
	vec3 m0_v (0,0,0);
	vec3 m1_v (0,0,2);

	vector<SubmodelCollision> colls =  linearInterpolation(
			0.0, 1.0,	//times t0, t1
			m0, m1,
			m0_pos, m1_pos,
			m0_v, m1_v
			);
	
	ASSERT_NE(0, colls.size());

	//all collisions happen at the same time
	for(auto c : colls)
	{
		EXPECT_FLOAT_EQ(colls[0].time, c.time);
	}

	//the time is at t0.5
	EXPECT_EQ(0.5, colls[0].time);

	//only edge-edge collisions have been detected
	for(auto c : colls)
	{
		EXPECT_TRUE(c.isEdgeEdge()) << c.feat0.type << "|" << c.feat1.type << '\n';
	}

	//from drawing the triangles we can see 4 edge-edge collisions
	//p
	//since the edge-step is only calculated once we expect 4 collisions
	EXPECT_EQ(4, colls.size());
}

TEST_F(test_collisionl2_linear_interpolation, vertex_face_only)
{
	Model m0(&mesh0);
	Model m1(&mesh2);
	vec3 m0_pos (0,0,0);
	vec3 m1_pos (0,0,-1);
	vec3 m0_v (0,0,0);
	vec3 m1_v (0,0,2);

	vector<SubmodelCollision> colls =  linearInterpolation(
			0.0, 1.0,	//times t0, t1
			m0, m1,
			m0_pos, m1_pos,
			m0_v, m1_v
			);
	
	ASSERT_NE(0, colls.size());

	//all collisions happen at the same time
	for(auto c : colls)
	{
		EXPECT_FLOAT_EQ(colls[0].time, c.time);
	}

	//the time is at t0.5
	EXPECT_EQ(0.5, colls[0].time);

	//only edge-edge collisions have been detected
	for(auto c : colls)
	{
		EXPECT_TRUE(c.isVertexFace()) << c.feat0.type << "|" << c.feat1.type << '\n';
	}

	//from drawing the triangles we can see 3 vertex-face collisions
	//since m0's vertices do not collide and no edges collide we count 3
	EXPECT_EQ(3, colls.size());
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
