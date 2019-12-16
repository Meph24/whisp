/*
 * Frustum.cpp
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "Frustum.h"

#include "IWorld.h"
#include "ITexture.h"

#include <iostream>

Frustum::Frustum()
{}

bool Frustum::inside(AABB bb,IWorld * w)
{
	for(int i=0;i<FRUSTUM_PLANE_COUNT;i++)
	{
		bool ret=planes[i].inside(bb,observerPos,w);
		//if(!ret) std::cout<<i<<" "<<ret<<planes[i].normal<<" "<<planes[i].distanceInMeters<<std::endl;
		if(!ret) return false;
	}
	return true;
}


void Frustum::debugDraw(ITexture * tex,IWorld * w)
{
	for(int i=0;i<FRUSTUM_PLANE_COUNT;i++)
	{
		vec3 normal=planes[i].normal;
		vec3 rightAngle=glm::cross(normal,vec3(0,1,0));
		rightAngle = glm::normalize(rightAngle);
		vec3 rightAngle2=glm::cross(normal,rightAngle);
		vec3 drawPoint=normal*(planes[i].distanceInMeters-1.0f);

		glEnable(GL_TEXTURE_2D);
		tex->bind();

		glPushMatrix();
		glColor3f(1.0f,0.5f, 0.0f);
		glTranslatef(drawPoint.x+1, drawPoint.y+1, drawPoint.z);
		glScalef(10,10,10);
		glBegin(GL_TRIANGLES);
		glTexCoord2f(1, 0);
		glVertex3f(rightAngle.x*0.2f, rightAngle.y*0.2f, rightAngle.z*0.2f);
		glTexCoord2f(0.5, 1);
		glVertex3f(normal.x,normal.y,normal.z);
		glTexCoord2f(0, 0);
		glVertex3f(-rightAngle.x*0.2f, -rightAngle.y*0.2f, -rightAngle.z*0.2f);

		glTexCoord2f(1, 0);
		glVertex3f(rightAngle2.x*0.2f, rightAngle2.y*0.2f, rightAngle2.z*0.2f);
		glTexCoord2f(0.5, 1);
		glVertex3f(normal.x,normal.y,normal.z);
		glTexCoord2f(0, 0);
		glVertex3f(-rightAngle2.x*0.2f, -rightAngle2.y*0.2f, -rightAngle2.z*0.2f);

		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

	}
}

Frustum::~Frustum()
{}

