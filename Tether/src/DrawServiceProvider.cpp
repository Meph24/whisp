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

void DrawServiceProvider::transformViewToGUI(float priority)
{
	glPushMatrix();
//	depthStatus=glIsEnabled(GL_DEPTH_TEST);
//	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	float prioScale=2.25f-priority;
	if(!cam)
	{
		glScalef(prioScale,prioScale,-prioScale);
		WarnErrReporter::notInitializedErr("camera not initialized (in transformViewToGUI)");
	}
	else
	{
		glScalef(cam->minView*prioScale*cam->zoom, cam->minView*prioScale*cam->zoom, -cam->minView*prioScale);
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
