/*
 * ZombieTree.cpp
 *
 *  Created on:	28.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ZombieTree.h"
#include <GL/glew.h>

Zombie_Tree::Zombie_Tree(vec3 position, ITexture* textureLog, ITexture* textureLeaves):
pos(position),tex1(textureLog),tex2(textureLeaves)
{
	d = 0.8f;//TODO
	h = 18;
}

Zombie_Tree::Zombie_Tree(vec3 position, flt diameter, flt height,  ITexture* textureLog, ITexture* textureLeaves):
d(diameter),h(height+8),pos(position),tex1(textureLog),tex2(textureLeaves)
{

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
	flt r = d*6;
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

void Zombie_Tree::draw()
{
	tex1->bind();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(pos.x, pos.y-8, pos.z);

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
	// TODO Auto-generated destructor stub
}

