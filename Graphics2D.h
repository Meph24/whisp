#pragma once
#include <GL\glew.h>


typedef struct
{
	float r;
	float g;
	float b;
} color;

class Graphics2D
{
	float * xCircle;
	float * yCircle;
	int segments;
	float maxX;
	float maxY;

public:
	float getMaxX();
	float getMaxY();
	void fillRect(float x, float y, float width, float height);
	void fillRect(float x, float y, float width, float height, color c);
	void feedRects(unsigned int count, float * x, float * y, float * width, float * height);
	void feedRects(unsigned int count, float * x, float * y, float * width, float * height, float * r, float * g, float * b);
	void feedRects(unsigned int count, float * x, float * y, float width, float height, float * r, float * g, float * b);
	void fillOval(float xCenter, float yCenter, float xRad, float yRad);
	void drawString(char * str,float x,float y);
	void setColor(float r, float g, float b);
	void endDraw();
	void beginDraw();
	Graphics2D(int circleSegments);
	~Graphics2D();
};

