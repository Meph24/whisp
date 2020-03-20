#include "../src/gjk.hpp"

#include <vector>
#include <utility>
#include "../src/GeoFeatures.hpp"

#include <gtest/gtest.h>

using std::pair;
using std::vector;

using gjk::MinkowskiGenerator;

TEST(test_gjk, MinkowskiGenerator)
{
	vector<Vertex> vertices0 = { Vertex(1.0f, 1.0f, 1.0f, 1.0f),
									  Vertex(-0.75f, -0.75f, 0.75f, 1.0f)};
	vector<Vertex> vertices1 = { Vertex(0.5f, 0.5f, 0.5f, 1.0f),
									  Vertex(-0.25f, -0.25f, -0.25f, 1.0f)	};
	vector<unsigned int> indices0 = {0};
	vector<unsigned int> indices1 = {0, 1};

	MinkowskiGenerator mg (
			vertices0.begin(), vertices0.end(), vertices1.begin(), vertices1.end(),
			indices0.begin(), indices0.end(), indices1.begin(), indices1.end()
			);


	vector<Vertex> expect_vertex = {	Vertex (0.5f, 0.5f, 0.5f, 0.0f),
										Vertex(1.25f, 1.25f, 1.25f, 0.0f)	};
	vector<pair<Vertex, Vertex>> expect_vertices = 
	{
		pair<Vertex, Vertex> (Vertex(1.0f, 1.0f, 1.0f, 1.0f), Vertex(0.5f, 0.5f, 0.5f, 1.0f)),
		pair<Vertex, Vertex> (Vertex(1.0f, 1.0f, 1.0f, 1.0f), Vertex(-0.25f, -0.25f, -0.25f,1.0f))
	};
	vector<pair<unsigned int, unsigned int>> expect_indices = 
	{
		pair<unsigned int, unsigned int> (0, 0), pair<unsigned int, unsigned int> (0, 1)
	};

	unsigned int i = 0;
	for(auto iter = mg.begin(); iter!=mg.end(); iter++)
	{
		auto indices = iter.indices();
		auto vertices = iter.vertices();
		auto vertex = iter.get();

		ASSERT_EQ(vertex, *iter);
		ASSERT_EQ(expect_vertex[i], vertex);

		ASSERT_EQ(expect_indices[i].first, indices.first);
		ASSERT_EQ(expect_indices[i].second, indices.second);

		ASSERT_EQ(expect_vertices[i].first, vertices.first);
		ASSERT_EQ(expect_vertices[i].second, vertices.second);

		++i;
	}
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
