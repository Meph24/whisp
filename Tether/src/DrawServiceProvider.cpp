/*
 * DrawServiceProvider.cpp
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "DrawServiceProvider.h"
#include <GL/glew.h>

DrawServiceProvider::DrawServiceProvider()
{}

DrawServiceProvider::~DrawServiceProvider()
{
}

vec3 DrawServiceProvider::getForwardViewVec()
{
	cam->getForwardVector();
}

void DrawServiceProvider::transformViewToGUI()
{
	glPushMatrix();
}

vec3 DrawServiceProvider::getUpViewVec()
{
	cam->getUpVector();
}

void DrawServiceProvider::revertView()
{
	glPopMatrix();
}
