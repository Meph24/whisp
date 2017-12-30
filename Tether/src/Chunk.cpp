/*
 * Chunk.cpp
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "Chunk.h"
float Chunk::getHeight(flt xh, flt yh)
{
	if((xh-x)<0) return defaultHeight*2;
	if((yh-y)<0) return defaultHeight*2;
	if((xh-x)>size) return defaultHeight*2;
	if((yh-y)>size) return defaultHeight*2;


	//TODO interpolate
	float innerX=(xh-x)-(int)(xh-x);
	float innerY=(yh-y)-(int)(yh-y);
	float h00=getH((int)(xh-x),(int)(yh-y));
	float h10=getH((int)(xh-x)+1,(int)(yh-y));
	float h11=getH((int)(xh-x)+1,(int)(yh-y)+1);
	float h01=getH((int)(xh-x),(int)(yh-y)+1);


	return (innerX*h10+(1-innerX)*h00)*(1-innerY)+(innerX*h11+(1-innerX)*h01)*innerY;
}
#include <iostream>
#include <cmath>
void Chunk::render(int lod)
{
	//std::cout<<"rendering x="<<x<<" y="<<y<<std::endl;

	float whiteness=avgHeight;
	whiteness=pow(whiteness/100.0f,1.5f);
	glColor3f(0.2f+0.8f*whiteness,0.8f+0.2f*whiteness,0.2f+0.8f*whiteness);

	//glColor3f(0.2f,0.8f,0.2f);
	int smallSize=size-1;

	/*int vertices=3*4*smallSize*smallSize/4;

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufID);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   vertices,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
*/

	if(lod==1)
	{

		for(int myY = 0 ; myY<smallSize ; myY++)
		{
			for(int myX = 0 ; myX<smallSize; myX++)
			{
				//float whiteness=getH(myX,myY);
				//whiteness=pow(whiteness/100.0f,1.5f);
				//glColor3f(0.2f+0.8f*whiteness,0.8f+0.2f*whiteness,0.2f+0.8f*whiteness);//fine color lags as hell
				glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f(0.5f,0.5f);
				glVertex3f(x+myX+0.5f,(getH(myX,myY)+getH(myX+1,myY)+getH(myX+1,myY+1)+getH(myX,myY+1))/4,y+myY+0.5f);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(x+myX,getH(myX,myY),y+myY);

				glTexCoord2f(1.0f,0.0f);
				glVertex3f(x+myX+1,getH(myX+1,myY),y+myY);

				glTexCoord2f(1.0f,1.0f);
				glVertex3f(x+myX+1,getH(myX+1,myY+1),y+myY+1);

				glTexCoord2f(0.0f,1.0f);
				glVertex3f(x+myX,getH(myX,myY+1),y+myY+1);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(x+myX,getH(myX,myY),y+myY);
				glEnd();
			}
		}
	}
	else
	{
		for(int myY = 0 ; myY<smallSize ; myY+=lod)
		{
			for(int myX = 0 ; myX<smallSize; myX+=lod)
			{
				glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f(0.5f*lod,0.5f*lod);
				glVertex3f(x+myX+0.5f*lod,getH(myX+lod/2,myY+lod/2),y+myY+0.5f*lod);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(x+myX,getH(myX,myY),y+myY);

				glTexCoord2f(lod,0.0f);
				glVertex3f(x+myX+lod,getH(myX+lod,myY),y+myY);

				glTexCoord2f(lod,lod);
				glVertex3f(x+myX+lod,getH(myX+lod,myY+lod),y+myY+lod);

				glTexCoord2f(0.0f,lod);
				glVertex3f(x+myX,getH(myX,myY+lod),y+myY+lod);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(x+myX,getH(myX,myY),y+myY);
				glEnd();
			}
		}
	}
}
#include "noise/noise.h"
using namespace noise;

Chunk::Chunk(int xStart, int yStart, int baseSize):
x(xStart),y(yStart),size(baseSize+1),avgHeight(0)
{
	module::Perlin myModule;
	myModule.SetSeed(420);
	myModule.SetOctaveCount(10);
	myModule.SetFrequency(0.00125);
	myModule.SetPersistence(0.75);
	myModule.SetLacunarity(1.5f);
	height=new float[size*size];
	for(int yrun=0;yrun<size;yrun++)
	{
		for(int xrun=0;xrun<size;xrun++)
		{
			float val=((float)myModule.GetValue(xrun+x, yrun+y, 0))*10.0f;
			height[yrun*size+xrun]=val*val;
			avgHeight+=val*val;
		}
	}
	avgHeight/=size*size;

	/*int smallSize=size-1;
	int vertices=3*4*smallSize*smallSize;
	glGenBuffers(1, &bufID);
	glBindBuffer(GL_ARRAY_BUFFER, bufID);
	GLfloat * buf=new GLfloat[vertices];
	int i=0;
	for(int myY = 0 ; myY<smallSize ; myY++)
	{
		for(int myX = 0 ; myX<smallSize; myX++)
		{
			buf[i]=x+myX+0.5f;
			buf[i+1]=(getH(myX,myY)+getH(myX+1,myY)+getH(myX+1,myY+1)+getH(myX,myY+1))/4;
			buf[i+2]=y+myY+0.5f;
			i+=3;
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices, buf, GL_STATIC_DRAW);
*/
	/*for(int i = 0 ; i<size*size ; i++)
	{
		height[i]=0;//(i%2+0.25f*(i%4))*0.5f+0.5f;//i*0.01f;
	}*/
}
inline float Chunk::getH(int xh, int yh)
{
	return height[yh*size+xh];
}

Chunk::~Chunk()
{
	delete height;
}

