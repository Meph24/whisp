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
#include "FloatSeconds.hpp"
#include "Perspective.hpp"

#include <GL/glew.h>

#include <iostream>

Zombie_Tree::Zombie_Tree(spacevec position)
{
	surviveClearing=true;
	bb=AABB(position);
	pos=position;
	v.set0();
	d = 0.8f;//TODO
	h = 18;
}

Zombie_Tree::Zombie_Tree(spacevec position, float diameter, float height, float diameterLeavse)
	: d(diameter)
	, h(height+8)
{
	surviveClearing=true;
	bb=AABB(position);
	pos=position;
	dLeaves = diameterLeavse;
	v.set0();
}

void Zombie_Tree::drawLog()
{
	float r = d/2;
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
	float r = d*dLeaves;
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

void Zombie_Tree::draw(const SimClock::time_point& draw_time,Frustum * viewFrustum,IWorld& iw, Perspective& perspective)
{
	float tickOffset = (float) FloatSeconds ( draw_time - last_ticked );

	if(!viewFrustum->inside(bb,iw))
	{	
		return;
	}

	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;
	vec3 interPosMeters=iw.toMeters(interPos);

	ITexture* tex1 = perspective.graphics_ressources.tree.get();
	tex1->bind();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(interPosMeters.x, interPosMeters.y-8, interPosMeters.z);

	drawLog();
	glRotatef(180,0,1,0);
	drawLog();

	glDisable(GL_TEXTURE_2D);

	ITexture* tex2 = perspective.graphics_ressources.leaves.get();
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
	std::cout<<"destroy"<<std::endl;
}

void Zombie_Tree::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
//	float seconds=t-lastTick;
	last_ticked=next_tick_begin;

	IWorld * iw=&tsp->world();
	ITerrain * it=tsp->getITerrain();

	pos=it->clip(pos,true,*iw);

	int temp = (h - rootSize) + (d * dLeaves);
	if(temp<rootSize) temp=rootSize;
	bb=AABB(pos, iw->fromMeters(vec3(d*dLeaves,temp,d*dLeaves)));
}

void Zombie_Tree::serialize(sf::Packet& p, bool complete)
{
	if(complete)
	{
		p<<pos;
	}
}

void Zombie_Tree::deserialize(sf::Packet& p, SyncableManager& sm)
{
}

Zombie_Tree::Zombie_Tree(sf::Packet& p,SyncableManager& sm)
{
	p>>pos;
	surviveClearing=true;
	bb=AABB(pos);
	v.set0();
	d = 0.8f;//TODO
	h = 18;
}

void Zombie_Tree::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
}

void Zombie_Tree::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
}

u32 Zombie_Tree::getClassID()
{
	return CLASS_ID_Zombie_Tree;
}
