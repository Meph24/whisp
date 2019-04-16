/*
 * EntityPlayer.h
 *
 *  Created on:	Apr 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITYPLAYER_H_
#define SRC_ENTITYPLAYER_H_
#include "Entity.h"
#include "Pushable.h"
class Zombie_MouseInput;
class Zombie_KeyInput;
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
class CameraTP;
class Frustum;
#include "BulletLikeSource.h"
class EntityPlayer: public Entity,public Pushable, public BulletLikeSource
{
	void setTP(bool on);

	bool isPerspective=false;
public:
	Zombie_MouseInput * mouseInp;
	Zombie_KeyInput * keyInp;

	CameraTP * cam;
	float minTPdist=2;
	float maxTPdist=20;

	float characterHeight=1.6f;
	spacelen characterHeightConv;

	float HP=-1;
	float maxHP=100;
	float speed=0;
	float hitmark=0;

	EntityPlayer(Timestamp spawnTime,spacevec startPos,sf::Window * w,float sensX,float sensY,float characterSpeed);
	~EntityPlayer();

	void changeTPdist(float amount);

	spacevec getCamPos();
	Frustum * newGetViewFrustum(ChunkManager * cm,float viewDistRestriction=-1);

	void applyPerspective(Timestamp t,bool fresh,ChunkManager * cm);//returns position that must be used for relative draws

	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp);

	virtual void tick(Timestamp t,TickServiceProvider * tsp);

	virtual void push(spacevec amount, TickServiceProvider* tsp);

	virtual void hitCallback(float dmg,bool kill,bool projDestroyed,HittableBulletLike * victim);

};

#include "CameraTP.h"
#include "Frustum.h"
#include "Zombie_MouseInput.h"
#include "Zombie_KeyInput.h"

#endif /* SRC_ENTITYPLAYER_H_ */
