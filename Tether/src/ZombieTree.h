/*
 * ZombieTree.h
 *
 *  Created on:	28.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ZOMBIETREE_H_
#define SRC_ZOMBIETREE_H_

#include "Entity.h"

#include "Spacevec.h"

class ITexture;

class Zombie_Tree: public Entity
{
	void drawLog();
	void drawLeaves();
	float dLeaves = 6;// >=1
	float rootSize = 8;
public:
	float d;
	float h;

	ITexture * tex1;
	ITexture * tex2;


	Zombie_Tree(spacevec position, ITexture* textureLog, ITexture* textureLeaves);
	Zombie_Tree(spacevec position, float diameter, float height, float diameterLeaves, ITexture* textureLog, ITexture* textureLeaves);
	~Zombie_Tree();
	virtual void draw(const SimClock::time_point& t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);
	virtual void tick(const SimClock::time_point& t,TickServiceProvider * tsp);
};

#endif /* SRC_ZOMBIETREE_H_ */
