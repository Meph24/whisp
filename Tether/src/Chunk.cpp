/*
 * Chunk.cpp
 *
 *  Created on:	22.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "Chunk.h"
spacelen Chunk::getHeight(flt xh, flt zh)
{
	if(xh<0) return defaultH*2.0f;
	if(zh<0) return defaultH*2.0f;
	if(xh>1) return defaultH*2.0f;
	if(zh>1) return defaultH*2.0f;

	int smallSize=size-1;
	xh*=smallSize;
	zh*=smallSize;
	float innerX=xh-(int)xh;
	float innerY=zh-(int)zh;
	float h00=getH((int)(xh),(int)(zh));
	float h10=getH((int)(xh)+1,(int)(zh));
	float h11=getH((int)(xh)+1,(int)(zh)+1);
	float h01=getH((int)(xh),(int)(zh)+1);

	return parent->fromMeters((innerX*h10+(1-innerX)*h00)*(1-innerY)+(innerX*h11+(1-innerX)*h01)*innerY);
}
#include <iostream>
#include <cmath>
void Chunk::render(int lod,spacevec camOffset)
{
	//std::cout<<"rendering x="<<x<<" y="<<y<<std::endl;
	vec3 relpos=parent->toMeters(base-camOffset);
	//if(relpos.y!=0)
	//	std::cout<<"rendering relpos.x="<<relpos.x<<" relpos.y="<<relpos.y<<std::endl;

	float whiteness=avgHeight;//a racist float
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

		for(int myZ = 0 ; myZ<smallSize ; myZ++)
		{
			for(int myX = 0 ; myX<smallSize; myX++)
			{
				//float whiteness=getH(myX,myZ);
				//whiteness=pow(whiteness/100.0f,1.5f);
				//glColor3f(0.2f+0.8f*whiteness,0.8f+0.2f*whiteness,0.2f+0.8f*whiteness);//fine color lags as hell
				glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f(0.5f,0.5f);
				glVertex3f(relpos.x+myX+0.5f,relpos.y+(getH(myX,myZ)+getH(myX+1,myZ)+getH(myX+1,myZ+1)+getH(myX,myZ+1))/4,relpos.z+myZ+0.5f);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(relpos.x+myX,relpos.y+getH(myX,myZ),relpos.z+myZ);

				glTexCoord2f(1.0f,0.0f);
				glVertex3f(relpos.x+myX+1,relpos.y+getH(myX+1,myZ),relpos.z+myZ);

				glTexCoord2f(1.0f,1.0f);
				glVertex3f(relpos.x+myX+1,relpos.y+getH(myX+1,myZ+1),relpos.z+myZ+1);

				glTexCoord2f(0.0f,1.0f);
				glVertex3f(relpos.x+myX,relpos.y+getH(myX,myZ+1),relpos.z+myZ+1);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(relpos.x+myX,relpos.y+getH(myX,myZ),relpos.z+myZ);
				glEnd();
			}
		}
	}
	else
	{
		for(int myZ = 0 ; myZ<smallSize ; myZ+=lod)
		{
			for(int myX = 0 ; myX<smallSize; myX+=lod)
			{
				glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f(0.5f*lod,0.5f*lod);
				glVertex3f(relpos.x+myX+0.5f*lod,relpos.y+getH(myX+lod/2,myZ+lod/2),relpos.z+myZ+0.5f*lod);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(relpos.x+myX,relpos.y+getH(myX,myZ),relpos.z+myZ);

				glTexCoord2f(lod,0.0f);
				glVertex3f(relpos.x+myX+lod,relpos.y+getH(myX+lod,myZ),relpos.z+myZ);

				glTexCoord2f(lod,lod);
				glVertex3f(relpos.x+myX+lod,relpos.y+getH(myX+lod,myZ+lod),relpos.z+myZ+lod);

				glTexCoord2f(0.0f,lod);
				glVertex3f(relpos.x+myX,relpos.y+getH(myX,myZ+lod),relpos.z+myZ+lod);

				glTexCoord2f(0.0f,0.0f);
				glVertex3f(relpos.x+myX,relpos.y+getH(myX,myZ),relpos.z+myZ);
				glEnd();
			}
		}
	}
}
#include "noise/noise.h"
using namespace noise;

Chunk::Chunk(spacevec basePos,int baseSize,ChunkManager * cm):
base(basePos),size(baseSize+1),avgHeight(0),parent(cm)
{
	defaultH=cm->fromMeters(defaultHeight*1.0f);
	module::Perlin myModule;
	myModule.SetSeed(420);
	myModule.SetOctaveCount(10);
	myModule.SetFrequency(0.00125);
	myModule.SetPersistence(0.75);
	myModule.SetLacunarity(1.5);
	height=new float[size*size];
	for(int zrun=0;zrun<size;zrun++)
	{
		for(int xrun=0;xrun<size;xrun++)
		{
			float val=((float)myModule.GetValue((double)xrun+baseSize*(double)base.x.floatpart+baseSize*(double)base.x.intpart, (double)zrun+baseSize*(double)base.z.floatpart+baseSize*(double)base.z.intpart, 0))*10.0f;
			height[zrun*size+xrun]=val*val;
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
	for(int myZ = 0 ; myZ<smallSize ; myZ++)
	{
		for(int myX = 0 ; myX<smallSize; myX++)
		{
			buf[i]=x+myX+0.5f;
			buf[i+1]=(getH(myX,myZ)+getH(myX+1,myZ)+getH(myX+1,myZ+1)+getH(myX,myZ+1))/4;
			buf[i+2]=y+myZ+0.5f;
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

void Chunk::tick(float time, TickServiceProvider* tsp)
{
	//TODO tick all entities
}

Chunk::~Chunk()
{
	delete height;
}

