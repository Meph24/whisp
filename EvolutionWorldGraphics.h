#pragma once
#include "EvolutionWorld.h"
#include "Graphics2D.h"
class EvolutionWorldGraphics
{
	EvolutionWorld & w;
	Graphics2D g;
	GLuint worldTex;
	int textureVariant=0;//0=float
	float * texF;
	unsigned int * texI;
	float * feedX;
	float * feedY;
	float * feedR;
	float * feedG;
	float * feedB;
	unsigned int rectsOnScreen=0;
	float midx, midy;
	float zoom=10;//1=one rect fills 1x1 area on screen
public:
	void prepareDraw(int tid, int tc);
	void draw();
	float screenToWorldX(float screenX);
	float screenToWorldY(float screenY);
	float worldToScreenX(float worldX);
	float worldToScreenY(float worldY);

	EvolutionWorldGraphics(EvolutionWorld & world);
	
	~EvolutionWorldGraphics();
};

