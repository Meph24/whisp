#ifndef SRC_COLLIDER_H_
#define SRC_COLLIDER_H_

#include <vector>
#include <chrono>

using std::chrono::microseconds;

class Entity;
class TickServiceProvider;
#include "Spacevec.h"
#include "DualPointer.h"
#include "GeoFeatures.hpp"
#include "Model.hpp"

using std::vector;

/**
 * @brief This is the Interface for any Entity that wants to be a Collider.
 * It provides all needed Information for any of the Collision Detection Algorithms.
 */
class Collider
{
public:
//	void registerPushCheck(Entity * e,float seconds,TickServiceProvider * tsp);
	void interact(Entity * self,DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp);

	virtual void collide(DualPointer<Collider> other, microseconds tick_duration, TickServiceProvider& tsp) = 0;

	//TODO optimize copys in vertices() (shared_ptr idea, custom smart pointer idea)
	/**
	 * @brief Get vertex-positions of this collider at a specific time.
	 *
	 * @param tick_time	Seconds relative to beginning of this tick (0.0) and only valid for values until its end.
	 *
	 * @return Collection of vertices relative to the colliders own origin/position at the given time (getPosition(tick_time)).
	 */
	virtual vector<Vertex> vertices (microseconds tick_time) = 0;


	/**
	 * @brief Get Edge-References to the vertices() at a specific time.
	 *
	 * @param tick_time	Seconds relative to beginning of this tick (0.0) and only valid for values until its end.
	 * @param indices An optional collection of indices. The index specifies a vertex index of an 3DObject Representation. The position/index of the index-entry in the given collection specifies the value and existing index in an Edge-Reference will be swapped out in the result of this function. This is done to match indices with a call of the given indices to the vertices() function.
	 */
	virtual vector<EdgeRef> edges () const = 0;
	/**
	 * @brief See virtual vector<EdgeRef> (float tick_time, const vector<unsigned int>* indices = nullptr);
	 */
	virtual vector<FaceRef> faces () const = 0;

	virtual std::vector<Model::ConvexPart> convexParts() const = 0;

	//TODO add metadata support or metadata support interface to the collision Reaction Call
	//metadata would include collision force, materials invoved, etc. in a more advanced version of this project also position
	//currently only time is important ... this part of the interface is therefor especially subject to change
	virtual void react(microseconds tick_time) = 0;

	virtual ~Collider() = default;
};

#endif /* SRC_COLLIDER_H_ */
