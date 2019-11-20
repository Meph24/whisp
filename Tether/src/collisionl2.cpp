#include "collisionl2.hpp"

#include "glmutils.hpp"
#include "static_intersection.hpp"

namespace collisionl2
{

CollisionFeature::CollisionFeature()
	: type(TYPE::Unknown)
	, mesh_indices({0})
{}

CollisionFeature::CollisionFeature(const VertexRef& vr)
	: type(TYPE::Vertex)
	, mesh_indices({vr,0,0})
{}

CollisionFeature::CollisionFeature(const EdgeRef& er)
	: type(TYPE::Edge)
	, mesh_indices({0})
{
	std::copy(er.begin(), er.end(), mesh_indices.begin());
}

CollisionFeature::CollisionFeature(const FaceRef& fr)
	: type(TYPE::Face)
{
	std::copy(fr.begin(), fr.end(), mesh_indices.begin());
}


SubmodelCollision::SubmodelCollision()
	: time(-1.0f)
{}

SubmodelCollision::SubmodelCollision	(	float time, vec3& pos, 
										CollisionFeature&& cf0,
										CollisionFeature&& cf1
									)
	: time(time)
	, pos(pos)
	, feat0(cf0), feat1(cf1)
{}


bool SubmodelCollision::operator==(const SubmodelCollision& other)
{
	return time == other.time;
}
bool SubmodelCollision::operator!=(const SubmodelCollision& other)
{
	return time != other.time;
}
bool SubmodelCollision::operator<(const SubmodelCollision& other)
{
	return time < other.time;
}
bool SubmodelCollision::operator>=(const SubmodelCollision& other)
{
	return !((*this)<other);
}
bool SubmodelCollision::operator>(const SubmodelCollision& other)
{
	return time > other.time;
}
bool SubmodelCollision::operator<=(const SubmodelCollision& other)
{
	return !((*this)>other);
}

bool SubmodelCollision::isVertexFace() const
{
	return	(	feat0.type == CollisionFeature::TYPE::Face
			&&	feat1.type == CollisionFeature::TYPE::Vertex
			)
			||
			(	feat1.type == CollisionFeature::TYPE::Face
			&&	feat0.type == CollisionFeature::TYPE::Vertex
			);
}

bool SubmodelCollision::isEdgeEdge() const
{
	return	(	feat0.type == CollisionFeature::TYPE::Edge
			&&	feat1.type == CollisionFeature::TYPE::Edge
			);
}


void linearInterpolationFaceVertex(
		vector<SubmodelCollision>& colls_out,
		float t0, float dt,
		Model& m0, Model& m1,
		const vec3& pos1_t0, const vec3& pos1_t1,
		bool reverse
	)
{
	vec3 pos, coeff;
	auto m0_faces = m0.faces();
	if(!m0_faces.size()) return;
	auto& m1_vertices = m1.vertices();
	if(!m1_vertices.size()) return;
	for(FaceRef fr0 : m0_faces)
	{
		Face f = fr0.face(m0.vertices().begin(), m0.vertices().end());

		unsigned int vertex_index = 0;
		for(Vertex p : m1_vertices)
		{
			//Vertex is dragged trhough time resuting in
			//	a Line of swept space
			Vertex p_t0 = p + pos1_t0;
			Vertex p_t1 = p + pos1_t1;

			if	(lineIntersectsTriangle
					(
						vec3(f[0]), vec3(f[1] - f[0]), vec3(f[2] - f[0]),
						vec3(p_t0), vec3(p_t1 - p_t0),
						&pos, &coeff
					)
				)
			{
				//construct a SubmodelCollision Object
				//the line was expanded in the temporal dimension
				//	==> z of coeff_out is our time percentile,
				//		as it is the line coefficient
				//	which we can calculate the time from
				colls_out.emplace_back	
				(	
					t0 + coeff.z*dt,	//time
					pos,				//pos
					(reverse)? CollisionFeature(vertex_index) : CollisionFeature(fr0),
					(reverse)? CollisionFeature(fr0) : CollisionFeature(vertex_index)
				);
			}
			vertex_index++;
		}
	}
}

void linearInterpolationEdgeEdge(
		vector<SubmodelCollision>& colls_out,
		float t0, float dt,
		Model& m0, Model& m1,
		const vec3& pos1_t0, const vec3& pos1_t1
	)
{
	vec3 pos, coeff;
	auto m0_edges = m0.edges();
	if(!m0_edges.size()) return;
	auto m1_edges = m1.edges();
	if(!m1_edges.size()) return;

	for(EdgeRef er0 : m0_edges)
	{
		Edge e0 = er0.edge(m0.vertices().begin(), m0.vertices().end());

		for(EdgeRef er1 : m1_edges)
		{
			//edge of m1 is dragged through time, resulting in
			//	a parallelogram of swept space
			//	with a base Edge of:
			Edge e1_t0 = 
				er1.edge(m1.vertices().begin(), m1.vertices().end())
				* glm::translate(pos1_t0);

			//and a translated vertex defining the opposite edge
			Vertex v1_t1 = 
				er1.edge(m1.vertices().begin(), m1.vertices().end())[0]
				+ pos1_t1;

			if	(	lineIntersectsParallelogram
					(	vec3(e1_t0[0]),
						vec3(e1_t0[1] - e1_t0[0]),
						vec3(v1_t1 - e1_t0[0]),
						vec3(e0[0]),	
						vec3(e0[1] - e0[0]),
						&pos, &coeff
					)
				)
			{
				//construct a SubmodelCollision Object
				//v1_t1 was given as the p2 of the parallelogram
				//this was the expansion of our temporal dimesion
				//	==> y of coeff_out is our time percentile
				//	which we can calculate the time from
				colls_out.emplace_back	(	t0 + coeff.y*dt,	//time
											pos,				//pos
											CollisionFeature(er0),
											CollisionFeature(er1)
										);
			}
		}
	}
}

vector<SubmodelCollision> linearInterpolation(
		float t0, float t1,
		Model& m0, Model& m1,
		const vec3& pos0, const vec3& pos1,
		const vec3& v0, const vec3& v1
		)
{
	vector<SubmodelCollision> colls;

	float dt = t1 - t0;

	// r0 here means we're relative to m0, while it resides at 0
	vec3 r0_pos1_t0 = pos1 - pos0;
	vec3 r0_v1 = v1 - v0;
	
	vec3 r0_pos1_t1 = r0_pos1_t0 + dt * r0_v1;

	//same is needed relative to m1, for the second face_vertex step	
	vec3 r1_pos0_t0 = pos0 - pos1;
	vec3 r1_v0 = v0 - v1;

	vec3 r1_pos0_t1 = r1_pos0_t0 + dt* r1_v0;

	
	linearInterpolationEdgeEdge	(	colls, 
									t0, dt, m0, m1, 
									r0_pos1_t0, r0_pos1_t1
								);

	linearInterpolationFaceVertex	(	colls, 
										t0, dt, m0, m1, 
										r0_pos1_t0, r0_pos1_t1
									);
	
	linearInterpolationFaceVertex	(	colls, 
										t0, dt, m1, m0, 
										r1_pos0_t0, r1_pos0_t1,
										true //give out features in reverse as m0 is m1 and m1 is m0 
									);
	
	return colls;
}

vector<SubmodelCollision> linearInterpolation(	
		float t0, float t1,
		IWorld* iworld,
		ModelEntity& m0, ModelEntity& m1
	)
{
	return linearInterpolation	(	t0, t1,
									m0.model(), m1.model(),
									iworld->toMeters(m0.pos),
									iworld->toMeters(m1.pos),
									iworld->toMeters(m0.v),
									iworld->toMeters(m1.v)
								);
}

} /* namespace collisionl2 */
