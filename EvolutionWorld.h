#pragma once

#include "PerformanceMeter.h"
#include "RandomDefines.h"
//#include "EvolutionWorldGraphics.h"


class EvolutionWorld
{
	friend class EvolutionWorldGraphics;

	float * growth1;//constant after generation
	float * growth2;//constant after generation
	float * growth3;//constant after generation
	float * friction;//constant after generation

	seed mySeed;

	typedef struct
	{
		float * food1;
		float * food2;
		float * food3;
		float * food4;//meat

		float growthMultiplier;
		float season;
	} tileData;

	const float rotMultiplier = 0.9999f;
	const float seasonSpeed = 1.0f/16384.0f;
	const float minGrowth = -0.5f;
	const float maxGrowth = 1.5f;

	const int sidelength = 32;
	const int size = sidelength*sidelength;//1024

	tileData read;
	tileData write;

	PerformanceMeter pm;

	void initConst();
	void deleteConst();
	void initTileBuffer(tileData * data);
	void deleteTileBuffer(tileData * data);

	void preCalc();
	void tileFoodCalc();
	void creatureCalc();
public:


	EvolutionWorld(seed s);
	~EvolutionWorld();


	void simStep();//must not be called simultaneously by multiple threads

};

