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

class Zombie_Tree
{
	void drawLog();
	void drawLeaves();
public:
	flt d;
	flt h;

	vec3 pos;
	ITexture * tex1;
	ITexture * tex2;


	Zombie_Tree(vec3 position, ITexture* textureLog, ITexture* textureLeaves);
	Zombie_Tree(vec3 position, flt diameter, flt height,  ITexture* textureLog, ITexture* textureLeaves);
	~Zombie_Tree();
	void draw();
};

#endif /* SRC_ZOMBIETREE_H_ */
