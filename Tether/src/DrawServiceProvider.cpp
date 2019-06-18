/*
 * DrawServiceProvider.cpp
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "DrawServiceProvider.h"
#include <GL/glew.h>
#include "WarnErrReporter.h"

DrawServiceProvider::DrawServiceProvider()
{}

DrawServiceProvider::~DrawServiceProvider()
{
}

vec3 DrawServiceProvider::getForwardViewVec()
{
	return cam->getForwardVector();
}

void DrawServiceProvider::transformViewToGUI()
{
	glPushMatrix();
//	depthStatus=glIsEnabled(GL_DEPTH_TEST);
//	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	if(!cam)
	{
		glScalef(1.0625f,1.0625f,-1.0625f);
		WarnErrReporter::notInitializedErr("camera not initialized (in transformViewToGUI)");
	}
	else
	{
		glScalef(cam->minView*1.25f*cam->zoom, cam->minView*1.25f*cam->zoom, -cam->minView*1.25f);
	}


}

vec3 DrawServiceProvider::getUpViewVec()
{
	return cam->getUpVector();
}

void DrawServiceProvider::setCam(ICamera3D* myCam)
{
	cam=myCam;
}

void DrawServiceProvider::revertView()
{
	glPopMatrix();
//	if(depthStatus) glEnable(GL_DEPTH_TEST);
}
