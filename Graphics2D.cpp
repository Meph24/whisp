#include "Graphics2D.h"
#include <cmath>
#include "TextureStatic2D.h"

Graphics2D::Graphics2D(int circleSegments, ITexture * myFont) :
font(myFont),
segments(circleSegments)
{
	if (segments < 3) segments = 0;
	if (segments != 0)
	{
		xCircle = new float[segments];
		yCircle = new float[segments];
		float mult = TAU / circleSegments;
		for (int i = 0; i < circleSegments; i++)
		{
			xCircle[i] = sin(i*mult);
			yCircle[i] = cos(i*mult);
		}
	}
	maxi.x = 1;//TODO correct
	maxi.y = 1;
	maxStack = 256;//TODO add optional parameter
	curMax = new point[maxStack];
	curMax[0] = maxi;
}
Graphics2D::Graphics2D(int circleSegments) :
segments(circleSegments)
{
	tps = new TexParamSet(2, 2);
	tps->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps->addF(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	tps->addF(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	font = new TextureStatic2D(tps, "./res/font.png");
	font->update();
	if (segments < 3) segments = 0;
	if (segments != 0)
	{
		xCircle = new float[segments];
		yCircle = new float[segments];
		float mult = TAU / circleSegments;
		for (int i = 0; i < circleSegments; i++)
		{
			xCircle[i] = sin(i*mult);
			yCircle[i] = cos(i*mult);
		}
	}
	maxi.x = 1;//TODO correct
	maxi.y = 1;
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
	if (tps != 0)
	{
		delete (TextureStatic2D *)font;
		delete tps;
	}
}

void Graphics2D::beginDraw()
{
	glLoadIdentity();
}

void Graphics2D::endDraw()
{
	glFlush();  // Render now
}

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
		//TODO throw exception
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
	glPopMatrix();
	stack--;
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

void Graphics2D::drawString(char * str,int len, float xll, float yll, float size)
{
	for (int i = 0; i < len; i++)
	{
		float tx = (str[i] % 16)/16.0f;
		float ty = (str[i] / 16)/16.0f;
		float dx = xll + i*charLen*size;
		float dy = yll;
		//TODO insert texture
		//font->bind();
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(tx, ty + 1.0f / 16.0f);
		glVertex2f(dx, dy);
		glTexCoord2f(tx + 1.0f / 16.0f, ty + 1.0f / 16.0f);
		glVertex2f(dx + charLen*size, dy);
		glTexCoord2f(tx + 1.0f / 16.0f, ty);
		glVertex2f(dx + charLen*size, dy + size);
		glTexCoord2f(tx, ty);
		glVertex2f(dx, dy + size);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}