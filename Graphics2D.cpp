#include "Graphics2D.h"
#include <cmath>


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
	maxi.x = 1;//TODO correct
	maxi.y = 1;
	maxStack = 256;//TODO add optional parameter
	curMax = new point[maxStack];
	curMax[0] = maxi;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j == i) emptyMat[i * 4 + j] = 1;
			else emptyMat[i * 4 + j] = 0;
		}
	}
}


Graphics2D::~Graphics2D()
{
	if (segments != 0)
	{
		delete[] xCircle;
		delete[] yCircle;
	}
	delete[] curMax;
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
	glLoadMatrixf(emptyMat);
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
	float oldSizex = curMax[stack].x;
	float oldSizey = curMax[stack].y;
	int locx = (snappoint % 3) - 1;
	int locy = (snappoint / 3) - 1;
	float oldSelectedx = oldSizex*locx;
	float oldSelectedy = oldSizey*locy;
	float newSelectedx = newSizex*locx;
	float newSelectedy = newSizey*locy;
	glPushMatrix();//save the matrix to create a new matrix from scratch
	glLoadMatrixf(emptyMat);
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
	//glutStrokeCharacter(GLUT_STROKE_ROMAN, str[0]);//TODO find alternative
}
