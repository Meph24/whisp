/*
 * EV_World.h
 *
 *  Created on:	Nov 20, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_EV_WORLD_H_
#define SRC_EV_WORLD_H_

#include "TextureDynamic2Dcpu.h"
#include "TexParamSet.h"
#include "ShortNames.h"

class EV_World
{
	int tickCounter;
	int lastDraw;
	void updateTexture();
	u8 getColorValue(float myFood);
public:

	float changeRate=0.01f;
	int xSize;
	int ySize;
	int foodKinds;
	float * food;//[x,y,foodKind]
	float * fertility;//[x,y,foodKind]
	TexParamSet * tps;
	u8 * texData;
	TextureDynamic2Dcpu * foodTex;

	//float * groundMoveability;//[x,y]



	EV_World(int xSize,int ySize,int foodKinds);
	~EV_World();
	void tick();
	void draw();
	void randomInit();
};

#endif /* SRC_EV_WORLD_H_ */
