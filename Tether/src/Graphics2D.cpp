#include "Graphics2D.h"
#include <cmath>
#include "TextureStatic2D.h"


#include <iostream>
#include "myAssert.h"

Graphics2D::Graphics2D(int circleSegments,float aspectRatio, ITexture * myFont) :
segments(circleSegments),
stack(0),
tps(myFont==0?2:0, myFont==0?2:0),
deleteTex(true)
{
	if(myFont)
	{
		deleteTex=false;
	}
	else
	{
		deleteTex=true;

		if (!tps.addI(GL_TEXTURE_WRAP_S, GL_REPEAT)) std::cout << "fuckshit" << std::endl;//TODO different error
		if (!tps.addI(GL_TEXTURE_WRAP_T, GL_REPEAT)) std::cout << "fuckshit" << std::endl;
		if (!tps.addF(GL_TEXTURE_MIN_FILTER, GL_NEAREST)) std::cout << "fuckshit" << std::endl;
		if (!tps.addF(GL_TEXTURE_MAG_FILTER, GL_NEAREST)) std::cout << "fuckshit" << std::endl;

		font = new TextureStatic2D(&tps, "./res/font.png");
		font->update();
	}
	initCircle();
	assert(aspectRatio>0);
	if(aspectRatio>=1)
	{
		maxi.x = aspectRatio;
		maxi.y = 1;
	}
	else
	{
		maxi.x=1;
		maxi.y=1/aspectRatio;
	}
	maxStack = 256;//TODO add optional parameter
	curMax = new point[maxStack];
	curMax[0] = maxi;
}

Graphics2D::~Graphics2D()
{
	if (segments != 0)
	{
		delete[] xCircle;
		delete[] yCircle;
	}
	delete[] curMax;
	if (deleteTex)
	{
		delete (TextureStatic2D *)font;
	}
}

void Graphics2D::initCircle()
{

	if (segments < 3) segments = 0;
	if (segments != 0)
	{
		xCircle = new float[segments];
		yCircle = new float[segments];
		float mult = TAU / segments;
		for (int i = 0; i < segments; i++)
		{
			xCircle[i] = sin(i*mult);
			yCircle[i] = cos(i*mult);
		}
	}
}

//void Graphics2D::beginDraw()
//{
//	glLoadIdentity();
//}
//
//void Graphics2D::endDraw()
//{
//	glFlush();  // Render now
//}

float Graphics2D::getMaxX()
{
	return maxi.x;
}

float Graphics2D::getMaxY()
{
	return maxi.y;
}
float Graphics2D::getCurMaxX()
{
	return curMax[stack].x;
}

float Graphics2D::getCurMaxY()
{
	return curMax[stack].y;
}

void Graphics2D::setSubsection(subsection * s)
{
	if (stack + 1 == maxStack)
	{
		//TODO error
	}
	else
	{
		stack++;
		curMax[stack] = s->maxi;
		glPushMatrix();
		glMultMatrixf(s->mat);
	}
}

void Graphics2D::resetLastSubsection()
{
	if(stack>0)
	{
		glPopMatrix();
		stack--;
	}
	else
	{
		//TODO error
	}
}
void Graphics2D::resetAllSubsections()
{
	while (stack > 0)
	{
		glPopMatrix();
		stack--;
	}
	stack = 0;
}

subsection Graphics2D::generateSubsection(float snapx, float snapy, float sizex, float sizey, int snappoint)
{
	subsection ret;
	float size = sizex<sizey?sizex:sizey;
	float newSizex = sizex / size;
	float newSizey = sizey / size;
	ret.maxi.x = newSizex;
	ret.maxi.y = newSizey;
	float oldSizex = curMax[stack].x;
	float oldSizey = curMax[stack].y;
	int locx = (snappoint % 3) - 1;
	int locy = (snappoint / 3) - 1;
	float oldSelectedx = oldSizex*locx;
	float oldSelectedy = oldSizey*locy;
	float newSelectedx = newSizex*locx;
	float newSelectedy = newSizey*locy;
	glPushMatrix();//save the matrix to create a new matrix from scratch
	glLoadIdentity();
	glTranslatef(oldSelectedx + snapx - newSelectedx*size*0.5f, oldSelectedy + snapy - newSelectedy*size*0.5f, 0.0f);
	glScalef(size*0.5f, size*0.5f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, ret.mat);
	glPopMatrix();//restore matrix
	return ret;
}

subsection Graphics2D::generateSubsection(float snapx, float snapy, float sizex, float sizey, int snappoint,float angle)
{
	subsection ret;
	float size = sizex<sizey ? sizex : sizey;
	float newSizex = sizex / size;
	float newSizey = sizey / size;
	ret.maxi.x = newSizex;
	ret.maxi.y = newSizey;
	float oldSizex = curMax[stack].x;
	float oldSizey = curMax[stack].y;
	int locx = (snappoint % 3) - 1;
	int locy = (snappoint / 3) - 1;
	float oldSelectedx = oldSizex*locx;
	float oldSelectedy = oldSizey*locy;
	float newSelectedx = newSizex*locx;
	float newSelectedy = newSizey*locy;
	glPushMatrix();//save the matrix to create a new matrix from scratch
	//glLoadMatrixf(emptyMat);
	glLoadIdentity();
	glTranslatef(oldSelectedx + snapx - newSelectedx*size*0.5f, oldSelectedy + snapy - newSelectedy*size*0.5f, 0.0f);
	glScalef(size*0.5f, size*0.5f, 1.0f);
	glRotatef(angle, 0, 0, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, ret.mat);
	glPopMatrix();//restore matrix
	return ret;
}
void Graphics2D::setColor(float r, float g, float b)
{
	glColor3f(r, g, b);
}

void Graphics2D::fillRect(float x, float y, float width, float height)
{
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void Graphics2D::fillRect(float x, float y, float width, float height,color c)
{
	glBegin(GL_QUADS);
	glColor3f(c.r, c.g, c.b);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

//TODO replace with image drawing
void Graphics2D::feedRects(unsigned int count, float * x, float * y, float * width, float * height)
{
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < count; i++)
	{
		float xd = x[i];
		float yd = y[i];
		float heightd = height[i];
		float widthd = width[i];//TODO optimize?
		glVertex2f(xd, yd);
		glVertex2f(xd + widthd, yd);
		glVertex2f(xd + widthd, yd + heightd);
		glVertex2f(xd, yd + heightd);
	}
	glEnd();
}
void Graphics2D::feedRects(unsigned int count, float * x, float * y, float * width, float * height, float * r, float * g, float * b)
{
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < count; i++)
	{
		glColor3f(r[i], g[i], b[i]);
		float xd = x[i];
		float yd = y[i];
		float heightd = height[i];
		float widthd = width[i];//TODO optimize?
		glVertex2f(xd, yd);
		glVertex2f(xd + widthd, yd);
		glVertex2f(xd + widthd, yd + heightd);
		glVertex2f(xd, yd + heightd);
	}
	glEnd();
}
void Graphics2D::feedRects(unsigned int count, float * x, float * y, float width, float height, float * r, float * g, float * b)
{
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < count; i++)
	{
		glColor3f(r[i], g[i], b[i]);
		float xd = x[i];
		float yd = y[i];
		glVertex2f(xd, yd);
		glVertex2f(xd + width, yd);
		glVertex2f(xd + width, yd + height);
		glVertex2f(xd, yd + height);
	}
	glEnd();
}

void Graphics2D::fillOval(float xCenter, float yCenter, float xRad, float yRad)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(xCenter,yCenter);
	for (int i = 0; i < segments;i++)
		glVertex2f(xCircle[i] * xRad + xCenter, yCircle[i] * yRad + yCenter);
	glVertex2f(xCircle[0] * xRad + xCenter, yCircle[0] * yRad + yCenter);
	glEnd();
}

void Graphics2D::drawString(const char * str,float xll,float yll, float size,int len,int maxLen)
{
	if(len==-1)
	{
		len=0;
		while(str[len]) len++;
	}
	if(maxLen!=-1) len=len>maxLen?maxLen:len;
	if (len < 1) return;
	font->bind();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for (int i = 0; i < len; i++)
	{
		float tx = (str[i] % 16)/16.0f;
		float ty = (str[i] / 16)/16.0f;
		float dx = xll + i*charLen*size;
		float dy = yll;

		glTexCoord2f(tx, ty + 1.0f / 16.0f);
		glVertex3f(dx, dy,1);

		glTexCoord2f(tx + 1.0f / 16.0f, ty + 1.0f / 16.0f);
		glVertex3f(dx + charLen*size, dy,1);

		glTexCoord2f(tx + 1.0f / 16.0f, ty);
		glVertex3f(dx + charLen*size, dy + size,1);

		glTexCoord2f(tx, ty);
		glVertex3f(dx, dy + size,1);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Graphics2D::drawString(std::string str, float xll, float yll, float size, int maxLen)
{
	drawString(str.c_str(),xll,yll,size,str.length(),maxLen);
}
