#ifndef SRC_ENTITYPLAYER_H_
#define SRC_ENTITYPLAYER_H_

#include <memory>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "BulletLikeSource.h"
#include "diamondMesh.hpp"
#include "Entity.h"
#include "Eye.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Pushable.h"
#include "SimClock.hpp"
#include "SimulationInputStatusSet.hpp"
#include "TopLevelInventory.h"
#include "Zombie_Gun.h"

using std::unique_ptr;
using std::vector;

class Zombie_MouseInput;
class Zombie_Gun;
class CameraTP;
class Frustum;
class Item;
class ItemContainer;
class ITexture;
struct SimulationUser;

class EntityPlayer: public Entity, public Pushable, public BulletLikeSource
{
	struct PrevInputStatus
	{
		SignalCounter inventory = 0;
		SignalCounter selection_up, selection_down;
		vec3 turn;
	}prev_input_status;

	SimulationUser* user_ = nullptr;

	Mesh player_mesh;
	Model player_model;

public:
	void setUser(SimulationUser* user = nullptr);
	const SimulationUser* user() const; //return 0/false if Entity is not controlled

	double score=0;

	Eye eye;

	float HP=-1;
	float maxHP=100;
	float speed=0;
	float hitmark=0;

	vector<unique_ptr<Zombie_Gun>> guns;
	Zombie_Gun* current_gun = nullptr;

	unique_ptr<Item> held_item;
	unique_ptr<Item> unused_inventory_slot;//contains other top-level inventories like backpack, jeans pockets, or directly attached items like sling
	TopLevelInventory* inventory();

	EntityPlayer(	SimClock::time_point spawn_time,
					spacevec startPos, 
 					float characterSpeed	);

	EntityPlayer(const EntityPlayer&) = delete;
	EntityPlayer& operator=(const EntityPlayer&) = delete;
	EntityPlayer(EntityPlayer&&) = delete;
	EntityPlayer& operator=(EntityPlayer&&) = delete;

	void changeTPdist(float amount);

	virtual void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum, IWorld& iw, Perspective& perspective);
	virtual void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp);
	virtual void push(spacevec amount, TickServiceProvider& tsp);
	virtual void hitCallback(float dmg,bool kill,bool projDestroyed,HittableBulletLike * victim);
	void selectWeapon(size_t selection);


	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	EntityPlayer(sf::Packet p, SyncableManager& sm);//deserialize constructor
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();
};

#endif /* SRC_ENTITYPLAYER_H_ */
