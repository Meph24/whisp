/*
 * DrawServiceProvider.cpp
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "DrawServiceProvider.h"
#include "WarnErrReporter.h"
#include "ICamera3D.h"
#include "Drawable.h"

#include <GL/glew.h>

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

float DrawServiceProvider::getAspectRatio()
{
	return cam->width/cam->height;
}

bool DrawServiceProvider::reinitGraphics()
{
	return false;//TODO
}

ITexture* DrawServiceProvider::suggestFont()
{
	return 0;
}

void DrawServiceProvider::registerTransparentCallback(float priority,Drawable* callbackRequester)
{
	callbackList.push_back(std::make_pair(priority,callbackRequester));
}

void DrawServiceProvider::doTransparentCallbacks(Timestamp t,Frustum * viewFrustum,ChunkManager* cm)
{
	std::sort(callbackList.begin(), callbackList.end());
	isTransparentPass=true;
	for(auto e : callbackList)
	{
		e.second->draw(t,viewFrustum,cm,this);
	}
	isTransparentPass=false;
}
