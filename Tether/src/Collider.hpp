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
	void interact(Entity * self,DualPointer<Collider> other, float time, TickServiceProvider& tsp);

	void saveState(const State& state);

	spacevec colSavedPos() const;
	spacevec colSavedV() const;
	
	/**
	 * @brief Set the persistent position of the collider.
	 *
	 * Implementations have to persist the by the interaction 
	 *  calculated pos in the derived class.
	 *
	 * @param newpos position to set
	 */
	virtual void colSetRealPos(const spacevec& newpos) = 0;

	/**
	 * @brief Set the persistent velocity of the collider.
	 *
	 * Implementations have to persist the by the interaction 
	 *	calculated v in the derived class.
	 *
	 * @param newv velocity to set
	 */
	virtual void colSetRealV(const spacevec& newv) = 0;


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
	virtual void collide(Collider* other, float time, TickServiceProvider& tsp) = 0;

	virtual ~Collider() = default;
};

#endif /* SRC_COLLIDER_H_ */
