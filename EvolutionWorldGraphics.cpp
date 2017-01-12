#include "EvolutionWorldGraphics.h"


EvolutionWorldGraphics::EvolutionWorldGraphics(EvolutionWorld & world) :w(world) , g(256)
{
	unsigned int feedSize = w.size;
	feedX = new float[feedSize * 5];
	feedY = feedX + feedSize;
	feedR = feedX + feedSize*2;
	feedG = feedX + feedSize*3;
	feedB = feedX + feedSize*4;
}


EvolutionWorldGraphics::~EvolutionWorldGraphics()
{
	delete[] feedX;
}

float EvolutionWorldGraphics::screenToWorldX(float screenX)
{
	return 0;//TODO fix
}
float EvolutionWorldGraphics::worldToScreenX(float worldX)
{
	return 0;//TODO fix

}
float EvolutionWorldGraphics::screenToWorldY(float screenY)
{

	return 0;//TODO fix
}
float EvolutionWorldGraphics::worldToScreenY(float worldY)
{

	return 0;//TODO fix
}

void EvolutionWorldGraphics::prepareDraw(int tid, int tc)
{
	if (textureVariant == 0)
	{
		//for (int i = 0;i<)
	}


	//+ is right up
	float llx = -g.getMaxX();//TODO put in screenToWorld() and worldToScreen()
	float lly = -g.getMaxY();
	float xhs = g.getMaxX();
	float yhs = g.getMaxY();
	float xsize = g.getMaxX() * 2;
	float ysize = g.getMaxY() * 2;
	float rux = g.getMaxX();
	float ruy = g.getMaxY();
	float mx = llx + xsize / 2;
	float my = lly + ysize / 2;

	int firstx=(int)screenToWorldX(llx);
	int lastx = (int)screenToWorldX(rux) + 1;//exclusive, ready for a "for" loop
	int firsty = (int)screenToWorldY(lly);
	int lasty = (int)screenToWorldY(ruy)+1;//exclusive, ready for a "for" loop
	if (firstx < 0)firstx = 0;
	if (firsty < 0)firsty = 0;
	if (lastx > w.sidelength) lastx = w.sidelength;
	if (lasty > w.sidelength) lasty = w.sidelength;
	int len = w.sidelength;

	if (tid == 0) rectsOnScreen = (lastx - firstx)*(lasty - firsty);

	float foodinv = (1.0f - w.rotMultiplier);

	for (int y = firsty + tid; y < lasty; y +=tc)
	{
		int i = firstx + firsty*len;
		float scy = worldToScreenY(y);
		for (int x = firstx; x < lastx; x++)//TODO optimize
		{
			feedR[i] = w.read.food1[i]*foodinv;
			feedG[i] = w.read.food2[i]*foodinv;
			feedB[i] = w.read.food3[i]*foodinv;
			feedX[i] = worldToScreenX(x);//TODO optimize
			feedY[i] = scy;
			i++;
		}
	}
}

void EvolutionWorldGraphics::draw()
{


	float size = 0;
	g.feedRects(rectsOnScreen, feedX, feedY, size, size,feedR,feedG,feedB);
}
