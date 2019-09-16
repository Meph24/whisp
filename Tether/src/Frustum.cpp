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


Frustum::Frustum()
{}

bool Frustum::inside(spacelen* bb,IWorld * w)
{
	for(int i=0;i<FRUSTUM_PLANE_COUNT;i++)
	{
		if(!planes[i].inside(bb,observerPos,w)) return false;
	}
	return true;
}

bool Frustum::inside(AABB bb,IWorld * w)
{
	return inside(&bb.low.x,w);
}

void Frustum::debugDraw(ITexture * tex,IWorld * w)
{
	for(int i=0;i<FRUSTUM_PLANE_COUNT;i++)
	{
		vec3 normal=planes[i].normal;
		vec3 rightAngle=crossProduct(normal,vec3(0,1,0));
		rightAngle.normalize();
		vec3 rightAngle2=crossProduct(normal,rightAngle);

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

