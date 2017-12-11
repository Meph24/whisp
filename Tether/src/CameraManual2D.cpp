/*
 * CameraManual2D.cpp
 *
 *  Created on:	Sep 5, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "CameraManual2D.h"

CameraManual2D::CameraManual2D(int resX,int resY)//:
//width(resX),height(resY)
{
	width=resX;
	height=resY;
}

CameraManual2D::~CameraManual2D()
{
	// TODO Auto-generated destructor stub
}

void CameraManual2D::apply()
{
	float pixelOffsetX=(((int)width+1)%2);
	float pixelOffsetY=(((int)height+1)%2);

	glMatrixMode(GL_MODELVIEW);

	/*
	 * 0 genau in der Mitte eines Pixels
	 * ->evtl verschiebung um einen halben Pixel nötig
	 *
	 */
	glTranslatef(pixelOffsetX/width,pixelOffsetY/height,0);//factor 2 eliminated from OpenGL (-1|1) and it being half a pixel
	glRotatef(gamma, 0, 0, 1);
	if(height>width)
	{
		glScalef((height/width)/zoom,1/zoom,1);
	}
	else
	{
		glScalef(1/zoom,(width/height)/zoom,1);
	}
	glTranslatef(-posX, -posY, -posZ);
}
