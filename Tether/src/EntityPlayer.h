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
#include "CameraTP.h"

class EntityPlayer: public Entity
{
	void setTP(bool on);

	bool isPerspective=false;
public:
	CameraTP * cam;
	float minTPdist=2;
	float maxTPdist=20;

	float characterHeight=1.6f;

	float HP=0;
	float maxHP=100;

	EntityPlayer(spacevec startPos,float screenH,float screenW);
	~EntityPlayer();

	void changeTPdist(float amount);

	spacevec applyPerspective(bool fresh,ChunkManager * cm);//returns position that must be used for relative draws

	virtual void draw(float tickOffset,spacevec observerPos,ChunkManager * cm,DrawServiceProvider * dsp);

	virtual void tick(float time,TickServiceProvider * tsp);
};

#endif /* SRC_ENTITYPLAYER_H_ */
