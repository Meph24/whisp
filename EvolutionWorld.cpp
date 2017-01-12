#include "EvolutionWorld.h"
#include <cmath>
#define TAU 6.283185307179586

EvolutionWorld::EvolutionWorld(seed s):pm(3)// , foo(x)
{
	mySeed=s;
	initConst();
	initTileBuffer(&read);
	initTileBuffer(&write);
}


EvolutionWorld::~EvolutionWorld()
{
	deleteConst();
	deleteTileBuffer(&read);
	deleteTileBuffer(&write);
}

void EvolutionWorld::initConst()
{
	growth1 = new float[size];
	growth2 = new float[size];
	growth3 = new float[size];
	friction = new float[size];
	//TODO fill with values
}

void EvolutionWorld::deleteConst()
{
	delete[] growth1;
	delete[] growth2;
	delete[] growth3;
	delete[] friction;
}

void EvolutionWorld::initTileBuffer(tileData * data)
{
	data->food1 = new float[size];
	data->food2 = new float[size];
	data->food3 = new float[size];
	data->food4 = new float[size];
	float avgFood = 0.5f / (1.0f - rotMultiplier);
	float * write = data->food1;
	float * read = growth1;
	for (int i = 0; i < size; i++)
	{
		write[i] = read[i] * avgFood;
	}
	write = data->food2;
	read = growth2;
	for (int i = 0; i < size; i++)
	{
		write[i] = read[i] * avgFood;
	}
	write = data->food3;
	read = growth3;
	for (int i = 0; i < size; i++)
	{
		write[i] = read[i] * avgFood;
	}
	write = data->food4;
	for (int i = 0; i < size; i++)
	{
		write[i] = avgFood*(1.0f/256.0f);
	}
}

void EvolutionWorld::deleteTileBuffer(tileData * data)
{
	delete[] data->food1;
	delete[] data->food2;
	delete[] data->food3;
	delete[] data->food4;
}

void EvolutionWorld::simStep()
{
	preCalc();
	//threads begin
	tileFoodCalc();
	//global sync
	creatureCalc();//creature brain logic performs atomic actions on tile buffer
	//threads end
	tileData temp = read;
	read = write;
	write = temp;
}

void EvolutionWorld::preCalc()
{
	write.season = fmod((read.season+seasonSpeed),1.0f);
	write.growthMultiplier = minGrowth+(maxGrowth-minGrowth)*0.5f*(1+(float)sin(write.season * TAU));
}

void EvolutionWorld::tileFoodCalc()
{
	float gm=write.growthMultiplier;
	for (int i = 0; i < size; i++)//TODO optimize
	{
		write.food4[i] = read.food4[i] * rotMultiplier;
		write.food1[i] = read.food1[i] * rotMultiplier + growth1[i] * gm;
		write.food2[i] = read.food2[i] * rotMultiplier + growth2[i] * gm;
		write.food3[i] = read.food3[i] * rotMultiplier + growth3[i] * gm;
	}
}

void EvolutionWorld::creatureCalc()
{

}