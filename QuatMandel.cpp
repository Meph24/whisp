/*
 * QuatMandel.cpp
 *
 *  Created on:	17.02.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "QuatMandel.h"

QuatMandel::QuatMandel(unsigned short size,int iterations):
resolution(size),iter(iterations)
{
	pixelData=new int[(size_t)size*size*size*3];
	mandelbrot=new int[(size_t)size*size*size];
	texturesX=new TextureDynamic2Dcpu * [size];
	texturesY=new TextureDynamic2Dcpu * [size];
	texturesZ=new TextureDynamic2Dcpu * [size];
	tps=new TexParamSet(2,2);
	tps->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps->addF(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	tps->addF(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

QuatMandel::~QuatMandel()
{
	delete[] pixelData;
	delete[] mandelbrot;
	delete[] texturesX;
	delete[] texturesY;
	delete[] texturesZ;
}

void QuatMandel::initTextures()
{
	for(size_t i=0;i<((size_t)resolution*resolution*resolution);i++)
	{
		pixelData[i]=0;
		mandelbrot[i]=0;
	}
	size_t setSize=(size_t)resolution*resolution*resolution;
	size_t texSize=(size_t)resolution*resolution;
	for(size_t i=0;i<((size_t)resolution);i++)
	{
		texturesZ[i]=new TextureDynamic2Dcpu(tps,pixelData+texSize*i,resolution,resolution);
		texturesY[i]=new TextureDynamic2Dcpu(tps,pixelData+setSize+texSize*i,resolution,resolution);
		texturesX[i]=new TextureDynamic2Dcpu(tps,pixelData+setSize*2+texSize*i,resolution,resolution);
	}
}

void QuatMandel::update()
{
	for(int i=0;i<resolution;i++)
	{
		int * pixels=pixelData+i*resolution*resolution;
		for(int y=0;y<resolution;y++)
		{
			for(int x=0;x<resolution;x++)
			{
				int myX=x;
				int myY=y;
				int myZ=i;
				pixels[y*resolution+x]=mandelbrot[myZ*resolution*resolution+myY*resolution+myX]==iter?(0xFF000000+0x100000*(myX%16)+0x1000*(myY%16)+0x10*(myZ%16)):0x0;
				myX=x;
				myY=i;
				myZ=y;
				pixels[resolution*resolution*resolution+y*resolution+x]=mandelbrot[myZ*resolution*resolution+myY*resolution+myX]==iter?(0xFF000000+0x100000*(myX%16)+0x1000*(myY%16)+0x10*(myZ%16)):0x0;
				myX=i;
				myY=x;
				myZ=y;
				pixels[resolution*resolution*resolution*2+y*resolution+x]=mandelbrot[myZ*resolution*resolution+myY*resolution+myX]==iter?(0xFF000000+0x100000*(myX%16)+0x1000*(myY%16)+0x10*(myZ%16)):0x0;
			}
		}
	}
	for(size_t i=0;i<((size_t)resolution);i++)
	{
		texturesX[i]->update();
		texturesY[i]->update();
		texturesZ[i]->update();
	}
}
#include <GL/glew.h>
void QuatMandel::draw()
{
	glPushMatrix();
	float scale=2048.0f*1.5f;
	glScalef(scale/resolution,scale/resolution,scale/resolution);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	for(int i=0;i<resolution;i++)
	{
		float iplus=i+0.5f;
		texturesX[i]->bind();
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(iplus,0,0);
		glTexCoord2f(0,1);
		glVertex3f(iplus,0,resolution);
		glTexCoord2f(1,1);
		glVertex3f(iplus,resolution,resolution);
		glTexCoord2f(1,0);
		glVertex3f(iplus,resolution,0);
		glEnd();
		texturesY[i]->bind();
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(0,iplus,0);
		glTexCoord2f(0,1);
		glVertex3f(0,iplus,resolution);
		glTexCoord2f(1,1);
		glVertex3f(resolution,iplus,resolution);
		glTexCoord2f(1,0);
		glVertex3f(resolution,iplus,0);
		glEnd();
		texturesZ[i]->bind();
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(0,0,iplus);
		glTexCoord2f(0,1);
		glVertex3f(0,resolution,iplus);
		glTexCoord2f(1,1);
		glVertex3f(resolution,resolution,iplus);
		glTexCoord2f(1,0);
		glVertex3f(resolution,0,iplus);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
#include "quat.h"
#include <iostream>
void QuatMandel::calc()
{
#pragma omp parallel for schedule(dynamic, 1)
	for(int z=0;z<resolution;z++)
	{
		for(int y=0;y<resolution;y++)
		{
			for(int x=0;x<resolution;x++)
			{
				quat dyn(x*4.0f/resolution-2.0f,y*4.0f/resolution-2.0f,0,0);//z*4.0f/resolution-2.0f,0);
				quat start(0,z*4.0f/resolution-2.0f,0,0);
				quat work=dyn;
				int i;
				for(i=0;i<iter;i++)
				{
					work=work*work+start;
					if(work.absSq()>4.0f) break;
				}
				/* Mandel
				quat start(x*4.0f/resolution-2.0f,y*4.0f/resolution-2.0f,z*4.0f/resolution-2.0f,0);
				quat work=start;
				int i;
				for(i=0;i<iter;i++)
				{
					work=work*work+start;
					if(work.absSq()>4.0f) break;
				}
				*/
				mandelbrot[z*resolution*resolution+y*resolution+x]=i;
			}
		}

		std::cout << "calc step "<<z<<"/"<<resolution<<std::endl;
	}
}
