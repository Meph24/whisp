#pragma once
#include <GL/glew.h>
#include "ITexture.h"

#define SNAP_SW 0
#define SNAP_W 3
#define SNAP_NW 6
#define SNAP_N 7
#define SNAP_NE 8
#define SNAP_E 5
#define SNAP_SE 2
#define SNAP_S 1
#define SNAP_MID 4


#include "MathStuff.h"


typedef struct
{
	float r;
	float g;
	float b;
} color;

typedef struct
{
	float x;
	float y;
} point;


typedef struct
{
	float mat[16];
	point maxi;//middle is  exactly (0|0)

} subsection;

//This class is not thread safe! This class does not fully support multiple instances of itself! Look at the code for exceptions to this rule.
class Graphics2D
{
	float * xCircle;
	float * yCircle;
	int segments;
	point maxi;
	point * curMax;
	unsigned int stack;
	unsigned int maxStack;
	TexParamSet tps;
	bool deleteTex;
	float charLen = 400.0f / 1024.0f;

	void initCircle();

public:
	ITexture * font;

public:
	float getMaxX();
	float getMaxY();
	float getCurMaxX();
	float getCurMaxY();


	/*
	The new space is guaranteed to be at least from -1 to 1 in both directions (scaling with respect to aspect ratio)
	The subsection  struct is sensitive to the current max x and y values so for different current max values you need multiple subsection struct instances
	This is only the case if a snappoint is set that depends on the changed value e.g. snap west depends on the max x but not the max y
	The snappoint specifies the point of the new system that corresponds to the snappoint of the old system + the specified coordinates
	e.g. snap west at (0|0) with size (1|1) means the new section fills a 1x1 region in the old space which has the westmost point exactly where the old space has its westmost point
	*/
	subsection generateSubsection(float snapx, float snapy, float sizex, float sizey, int snappoint);
	subsection generateSubsection(float snapx, float snapy, float sizex, float sizey, int snappoint, float angle);
	

	void setSubsection(subsection * s);
	void resetLastSubsection();
	void resetAllSubsections();

	void fillRect(float x, float y, float width, float height);
	void fillRect(float x, float y, float width, float height, color c);
	void feedRects(unsigned int count, float * x, float * y, float * width, float * height);
	void feedRects(unsigned int count, float * x, float * y, float * width, float * height, float * r, float * g, float * b);
	void feedRects(unsigned int count, float * x, float * y, float width, float height, float * r, float * g, float * b);
	void fillOval(float xCenter, float yCenter, float xRad, float yRad);
	void drawString(char * str,int len,float xll,float yll, float size);
	void setColor(float r, float g, float b);
	void endDraw();
	void beginDraw();
	Graphics2D(int circleSegments);
	Graphics2D(int circleSegments, ITexture * myFont);
	~Graphics2D();
};

