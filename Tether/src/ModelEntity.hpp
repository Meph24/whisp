#ifndef ENTITYDIAMOND_HPP
#     define ENTITYDIAMOND_HPP

#include "Entity.h"

#include "Spacevec.h"

#include "Model.hpp"

#include "Collider.hpp"

#include "collisionl2.hpp"

#include "glmutils.hpp"
#include <glm/glm.hpp>

#include "gjk.hpp"

using glm::vec3;


class ModelEntity : public Entity
{
	const Model& m_model;

	spacevec tick_begin_pos, tick_begin_v;

public:
	ModelEntity(const Model& model);
	~ModelEntity();

	const Model& model();
	
	virtual void draw(	const SimClock::time_point& draw_time,
						Frustum* viewFrustum, 
						IWorld& iw,
						Perspective& perspective
					 );

	/**
	 * @brief Calculates the AABB for the managed Model in Space.
	 * 
	 * This calculation only is successfull, if changes on pos and v
	 *	of this ModelEntity have not been done already in the current
	 *	tick!
	 *
	 * @param tick_seconds The time-length of the current tick in seconds.
	 * @param tsp A Tick Service Provider for relativation.
	 *
	 * @return AABB for the current tick.
	 */
	AABB aabb(float tick_seconds, TickServiceProvider* tsp);

	virtual void tick(	const SimClock::time_point& next_tick_begin,
						TickServiceProvider* tsp
					 );

	//ColliderInterface
	
	vector<Model::ConvexPart> convexParts() const;
	vector<Vertex> vertices (float tick_time) const;
	vector<EdgeRef> edges(float tick_time) const;
	vector<FaceRef> faces(float tick_time) const;


	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();
};

#endif /* ENTITYDIAMOND_HPP */
