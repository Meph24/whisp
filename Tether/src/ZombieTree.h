/*
 * ZombieTree.h
 *
 *  Created on:	28.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ZOMBIETREE_H_
#define SRC_ZOMBIETREE_H_
#include "MatrixLib.h"
#include "ITexture.h"
#include "Entity.h"

class Zombie_Tree: public Entity
{
	void drawLog();
	void drawLeaves();
	flt dLeaves = 6;// >=1
	flt rootSize = 8;
public:
	flt d;
	flt h;

	ITexture * tex1;
	ITexture * tex2;


	Zombie_Tree(spacevec position, ITexture* textureLog, ITexture* textureLeaves);
	Zombie_Tree(spacevec position, flt diameter, flt height, flt diameterLeaves, ITexture* textureLog, ITexture* textureLeaves);
	~Zombie_Tree();
	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp);
	virtual void tick(Timestamp t,TickServiceProvider * tsp);
};

#endif /* SRC_ZOMBIETREE_H_ */
