#pragma once

#include "BrainDefines.h"

class Creature
{
	float * x;
	float * y;
	float * energy;
	unsigned int alive = 0;
	unsigned int maxAlive;
	unsigned int * usedMemoryPart;
	unsigned int * occupied;//bit=0 occupied; bit=1 free //TODO initialize
	unsigned int maxMemory;
	float * memory;

	float * getMyMemory(ID creatureID);
	void setOccupied(ID newCreatureID);
	void setUnoccupied(ID oldCreatureID);
	ID getUnoccupiedID();

public:
	Creature(unsigned int maxCreatureCount,unsigned int maxMemoryPerCreature);
	~Creature();
};