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

void linearInterpolation_R0_FaceVertex(
		vector<SubmodelCollision>& colls_out,
		float t0, float t1,
		const Collider& c0, const Collider& c1,
		const vec3& r0_c1_pos_t0, const vec3& r0_c1_pos_t1
	)
{
	float dt = t1 - t0;
	vec3 pos, coeff;
	const auto c0_faces = c0.faces(t0);
	if(c0_faces.empty()) return;
	const auto c1_vertices = c1.vertices(t0);
	if(!c1_vertices.empty()) return;

	const auto& c0_vertices = c0.vertices(t0);
	for(FaceRef fr0 : c0_faces)
	{
		Face f = fr0.face(c0_vertices.begin(), c0_vertices.end());

		unsigned int vertex_index = 0;
		for(Vertex p : c1_vertices)
		{
			//Vertex is dragged trhough time resuting in
			//	a Line of swept space
			if	(lineIntersectsTriangle
					(
						vec3(f[0]), vec3(f[1] - f[0]), vec3(f[2] - f[0]),
						vec3(p + r0_c1_pos_t0), vec3(p + r0_c1_pos_t1),
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
					CollisionFeature(fr0),
				   	CollisionFeature(vertex_index)
				);
			}
			vertex_index++;
		}
	}
}

void linearInterpolation_R0_EdgeEdge(
		vector<SubmodelCollision>& colls_out,
		float t0, float t1,
		const Collider& c0, const Collider& c1,
		const vec3& r0_c1_pos_t0, const vec3& r0_c1_pos_t1
	)
{
	float dt = t1 - t0;
	vec3 pos, coeff;

	const auto c0_edges = c0.edges(t0);
	if(c0_edges.empty()) return;
	const auto c1_edges = c1.edges(t0);
	if(c1_edges.empty()) return;

	const auto c0_vertices = c0.vertices(t0);
	const auto c1_vertices = c1.vertices(t0);

	for(EdgeRef er0 : c0_edges)
	{
		Edge e0 = er0.edge(c0_vertices.begin(), c0_vertices.end());

		for(EdgeRef er1 : c1_edges)
		{
			//edge of m1 is dragged through time, resulting in
			//	a parallelogram of swept space
			//	with a base Edge of:
			Edge e1_t0 = 
				er1.edge(c1_vertices.begin(), c1_vertices.end())
				* glm::translate(r0_c1_pos_t0);

			//and a translated vertex defining the opposite edge
			Vertex v1_t1 = 
				er1.edge(c1_vertices.begin(), c1_vertices.end())[0]
				+ r0_c1_pos_t1;

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

vector<SubmodelCollision> linearInterpolation_R0(
		float t0, float t1, IWorld* iworld,	
		const Collider& c0, const Collider& c1
		)
{
	vector<SubmodelCollision> colls;

	spacevec c0_pos, c1_pos;
	vec3 c0_dpos, c1_dpos;

	c0_pos = c0.getPosition(t0);
	c1_pos = c1.getPosition(t0);

	c0_dpos = iworld->toMeters(c0.getPosition(t1) - c0_pos);
	c1_dpos = iworld->toMeters(c1.getPosition(t1) - c1_pos);


	//relative r[Relative collider number]_c[Collider]_[Collider member]_[at time]
	vec3 r0_c1_pos_t0 = iworld->toMeters(c1_pos - c0_pos);
	vec3 r0_c1_pos_t1 =	r0_c1_pos_t0 + c1_dpos;

	vec3 r1_c0_pos_t0 = iworld->toMeters(c0_pos - c1_pos);
	vec3 r1_c0_pos_t1 = r1_c0_pos_t0 + c0_dpos;


	
	linearInterpolation_R0_EdgeEdge	(	colls, 
									t0, t1, c0, c1, 
									r0_c1_pos_t0, r0_c1_pos_t1
								);

	linearInterpolation_R0_FaceVertex(	colls, 
										t0, t1, c0, c1, 
										r0_c1_pos_t0, r0_c1_pos_t1
									);

	vector<SubmodelCollision> temp_colls;	
	linearInterpolation_R0_FaceVertex	(	temp_colls, 
										t0, t1, c1, c0,
										r1_c0_pos_t0, r1_c0_pos_t1
									);
	
	//swap the features in the output of the call with switched out Colliders
	for(SubmodelCollision& sc : temp_colls)
	{
		//the positions are stored relative to c1, while function demands relativity to c0 => rerelativate to c0
		sc.pos = r0_c1_pos_t0 + sc.pos;
		//Collision features are stored in reverse in this last call to FaceVertex Interpolation
		std::swap(sc.feat0, sc.feat1);
	}
	//append vector
	colls.insert(colls.end(), temp_colls.begin(), temp_colls.end());
	
	return colls;
}

} /* namespace collisionl2 */
