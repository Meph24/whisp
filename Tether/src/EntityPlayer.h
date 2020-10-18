/*
 * EntityPlayer.h
 *
 *  Created on:	Apr 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITYPLAYER_H_
#define SRC_ENTITYPLAYER_H_

#include "BulletLikeSource.h"
#include "Pushable.h"
#include "Entity.h"
#include "SimClock.hpp"


class Zombie_MouseInput;
class Zombie_Gun;
class CameraTP;
class Frustum;
class Item;
class ItemContainer;
class ITexture;


#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "SimulationInputStatusSet.hpp"
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;


class EntityPlayer: public Entity,public Pushable, public BulletLikeSource
{
	void setTP(bool on);

	bool isPerspective=false;

	SignalCounter prev_inventory_signal;
	vec3 prev_wanted_turn;
public:

	float defaultZoom=1;

	int score=0;

	//Zombie_MouseInput * mouseInp;

	CameraTP * cam;
	float minTPdist=2;
	float maxTPdist=20;

	float characterEyeHeight=1.6f;
	spacevec characterEyeOffset;

	float HP=-1;
	float maxHP=100;
	float speed=0;
	float hitmark=0;

	vector<unique_ptr<Zombie_Gun>> guns;
	Zombie_Gun* current_gun = nullptr;

	Item * heldItem;//to browse the inventory, "heldItem" is switched with "inventory"
	Item * inventory;//contains other top-level inventories like backpack, jeans pockets, or directly attached items like sling

	EntityPlayer(SimClock::time_point spawn_time,spacevec startPos,sf::Window * w,float sensX,float sensY,float characterSpeed);
	~EntityPlayer();

	void changeTPdist(float amount);

	spacevec getCamPos();
	Frustum * newFrustumApplyPerspective(SimClock::time_point t,bool fresh,TickServiceProvider * tsp,float viewDistRestriction=-1);

	virtual void draw(const SimClock::time_point& draw_time,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);

	virtual void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp);

	virtual void push(spacevec amount, TickServiceProvider& tsp);

	virtual void hitCallback(float dmg,bool kill,bool projDestroyed,HittableBulletLike * victim);


	void selectWeapon(size_t selection);
	void trigger(bool pulled, SimClock::time_point now,ITexture * tex,IWorld& iw);

};

#endif /* SRC_ENTITYPLAYER_H_ */
