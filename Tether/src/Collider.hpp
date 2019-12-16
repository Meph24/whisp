#ifndef SRC_COLLIDER_H_
#define SRC_COLLIDER_H_

class Entity;
class TickServiceProvider;
#include "Spacevec.h"
#include "DualPointer.h"
#include "Model.hpp"

class ModelEntity;

class Collider
{
public:
	struct State
	{
		spacevec pos;
		spacevec v;
	};
private:
	State old_state;
public:
//	void registerPushCheck(Entity * e,float seconds,TickServiceProvider * tsp);
	void interact(Entity * self,DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp);

	void saveState(const State& state);

	spacevec colSavedPos() const;
	spacevec colSavedV() const;
	
	//Setting the pos and velocity of the entity via the pointers of the parameters

	/**
	 * @brief Gets the model from implementing class.
	 *
	 * @return Model*
	 */
	virtual Model* colModel() = 0;

	/**
	 * @brief Collides this collider and another together.
	 *
	 * @param other the other collider to collide with
	 */
	virtual void collide(DualPointer<Collider> other, float time, TickServiceProvider& tsp) = 0;

	/**
	 * @brief React to when a collision happens.
	 *
	 * This part of the interface will most likely change in the future to accomodate parameters like the position of the collision and whatever.
	 */
	virtual void colReact() = 0;

	virtual ~Collider() = default;
};

#endif /* SRC_COLLIDER_H_ */
