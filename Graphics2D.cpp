#include "Graphics2D.h"
#include <cmath>

#define TAU 6.283185307179586

Graphics2D::Graphics2D(int circleSegments)
{
	segments = circleSegments;
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
	maxX = 1;
	maxY = 1;
}


Graphics2D::~Graphics2D()
{
	if (segments != 0)
	{
		delete[] xCircle;
		delete[] yCircle;
	}
}

void Graphics2D::beginDraw()
{

}

void Graphics2D::endDraw()
{
	glFlush();  // Render now
}

float Graphics2D::getMaxX()
{
	return maxX;
}

float Graphics2D::getMaxY()
{
	return maxY;
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

void Graphics2D::drawString(char * str, float x, float y)
{
	//glutStrokeCharacter(GLUT_STROKE_ROMAN, str[0]);
}