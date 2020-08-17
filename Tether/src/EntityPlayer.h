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


class Zombie_MouseInput;
class Zombie_KeyInput;
class Zombie_Gun;
class CameraTP;
class Frustum;
class Item;
class ItemContainer;
class ITexture;


#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


class EntityPlayer: public Entity,public Pushable, public BulletLikeSource
{
	void setTP(bool on);

	bool isPerspective=false;
public:

	float defaultZoom=1;

	int score=0;

	Zombie_MouseInput * mouseInp;
	Zombie_KeyInput * keyInp;

	CameraTP * cam;
	float minTPdist=2;
	float maxTPdist=20;

	float characterEyeHeight=1.6f;
	spacevec characterEyeOffset;

	float HP=-1;
	float maxHP=100;
	float speed=0;
	float hitmark=0;

	//TODO replace this section
	int wCount;
	Zombie_Gun ** guns;
	int currentGun=0;

	Item * heldItem;//to browse the inventory, "heldItem" is switched with "inventory"
	Item * inventory;//contains other top-level inventories like backpack, jeans pockets, or directly attached items like sling

	EntityPlayer(Timestamp spawnTime,spacevec startPos,sf::Window * w,float sensX,float sensY,float characterSpeed);
	~EntityPlayer();

	void changeTPdist(float amount);

	spacevec getCamPos();
	Frustum * newFrustumApplyPerspective(Timestamp t,bool fresh,TickServiceProvider * tsp,float viewDistRestriction=-1);

	//void applyPerspective(Timestamp t,bool fresh,ChunkManager * cm);

	virtual void draw(Timestamp t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);

	virtual void tick(Timestamp t,TickServiceProvider * tsp);

	virtual void push(spacevec amount, TickServiceProvider& tsp);

	virtual void hitCallback(float dmg,bool kill,bool projDestroyed,HittableBulletLike * victim);


	void switchWeapon(int dir);
	void trigger(bool pulled,Timestamp now,ITexture * tex,IWorld& iw);
};

#endif /* SRC_ENTITYPLAYER_H_ */
