/*
 * EV_World.cpp
 *
 *  Created on:	Nov 20, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "EV_World.h"
#include <iostream>

EV_World::EV_World(int xSize,int ySize,int foodKinds):
xSize(xSize),ySize(ySize),foodKinds(foodKinds),tickCounter(0),lastDraw(-1)
{
	food=new float[xSize*ySize*foodKinds];
	texData=new unsigned char[xSize*ySize*4];
	tps=new TexParamSet(2,2);
	tps->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps->addF(GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST);
	tps->addF(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	foodTex=new TextureDynamic2Dcpu(tps,texData,xSize,ySize);
}

EV_World::~EV_World()
{
	delete foodTex;
	delete tps;
	delete texData;
	delete food;
}

void EV_World::tick()
{
	tickCounter++;
	//tick
	tickCounter++;
}

void EV_World::updateTexture()
{
	u64 len=xSize*ySize;
	for(u64 i = 0 ; i<len ; i++)
	{
		if(foodKinds>0) texData[i*4]=getColorValue(food[i*foodKinds]);
		if(foodKinds>1) texData[i*4+1]=getColorValue(food[i*foodKinds+1]);
		if(foodKinds>2) texData[i*4+2]=getColorValue(food[i*foodKinds+2]);
	}
}

u8 EV_World::getColorValue(float myFood)
{
	myFood*=256;
	int ret=int(myFood);
	if(ret>255) ret=255;
	if(ret<0) ret=0;
	return ret;
}

void EV_World::draw()
{
	if(tickCounter%2)
		std::cout<<"drawing parallel to ticking not implemented yet!!!"<<std::endl;
	if(tickCounter!=lastDraw)
	{
		lastDraw=tickCounter;
		updateTexture();
		foodTex->update();
	}
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);//ground
	glVertex3f(xSize/2, 0, ySize/2);

	glTexCoord2f(0, 1);
	glVertex3f(xSize/2, 0, -ySize/2);

	glTexCoord2f(0, 0);
	glVertex3f(-xSize/2, 0, -ySize/2);

	glTexCoord2f(1, 0);
	glVertex3f(-xSize/2, 0, ySize/2);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}
