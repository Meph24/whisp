/*
 * ZombieTree.cpp
 *
 *  Created on:	28.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ZombieTree.h"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
#include "ITexture.h"
#include "Frustum.h"

#include <GL/glew.h>

#include <iostream>

Zombie_Tree::Zombie_Tree(spacevec position, ITexture* textureLog, ITexture* textureLeaves):
tex1(textureLog),tex2(textureLeaves)
{
	surviveClearing=true;
	bb=AABB(position);
	pos=position;
	v.set0();
	d = 0.8f;//TODO
	h = 18;
}

Zombie_Tree::Zombie_Tree(spacevec position, flt diameter, flt height, flt diameterLeavse,  ITexture* textureLog, ITexture* textureLeaves):
d(diameter),h(height+8),tex1(textureLog),tex2(textureLeaves)
{
	surviveClearing=true;
	bb=AABB(position);
	pos=position;
	dLeaves = diameterLeavse;
	v.set0();
}

void Zombie_Tree::drawLog()
{
	flt r = d/2;
	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(0,0);
	glVertex3f(-r,0,-r);

	glTexCoord2f(1,0);
	glVertex3f(r,0,-r);

	glTexCoord2f(1,h/d);
	glVertex3f(r,h,-r);

	glTexCoord2f(0,h/d);
	glVertex3f(-r,h,-r);

	glTexCoord2f(-1,h/d);
	glVertex3f(-r,h,r);

	glTexCoord2f(-1,0);
	glVertex3f(-r,0,r);

	glEnd();
}

void Zombie_Tree::drawLeaves()
{
	flt r = d*dLeaves;
	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(0,0);
	glVertex3f(-r,r,0);

	glTexCoord2f(1,0);
	glVertex3f(r,r,0);

	glTexCoord2f(1,1);
	glVertex3f(r, -r,0);

	glTexCoord2f(0,1);
	glVertex3f(-r,-r,0);

	glEnd();
}

void Zombie_Tree::draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp)
{
	float tickOffset=t-lastTick;

	if(!viewFrustum->inside(bb,cm))
	{	
		return;
	}

	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;
	vec3 interPosMeters=cm->toMeters(interPos);

	tex1->bind();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(interPosMeters.x, interPosMeters.y-8, interPosMeters.z);

	drawLog();
	glRotatef(180,0,1,0);
	drawLog();

	glDisable(GL_TEXTURE_2D);

	tex2->bind();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glTranslatef(0, h, 0);
	for(int i=0;i < 6;i++)
	{
		drawLeaves();
		glRotatef(30,0,1,0);
	}

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}


Zombie_Tree::~Zombie_Tree()
{
	//Textures are not owned by tree
}

void Zombie_Tree::tick(Timestamp t, TickServiceProvider* tsp)
{
//	float seconds=t-lastTick;
	lastTick=t;

	ChunkManager * cm=tsp->getChunkManager();

	pos=cm->clip(pos,true);

	int temp = (h - rootSize) + (d * dLeaves);
	if(temp<rootSize) temp=rootSize;
	bb=AABB(pos, cm->fromMeters(vec3(d*dLeaves,temp,d*dLeaves)));
}
