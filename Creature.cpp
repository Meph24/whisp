#include "Creature.h"


Creature::Creature(unsigned int maxCreatureCount,unsigned int maxMemoryPerCreature)
{
	maxAlive = maxCreatureCount;
	maxMemory = maxMemoryPerCreature;
	x = new float[maxAlive*4+(maxAlive+31)/32];
	y = x + maxAlive;
	energy = x + maxAlive * 2;
	usedMemoryPart =(unsigned int*)( x + maxAlive * 3);//dirty: blame HL65536
	occupied = (unsigned int*)(x + maxAlive * 4);//dirty: blame HL65536
	memory = new float[maxAlive*maxMemory];
}


Creature::~Creature()
{
	delete[] x;
	delete[] memory;
}

float * Creature::getMyMemory(ID creatureID)
{
	return memory + maxMemory*creatureID;
}


void Creature::setOccupied(ID newCreatureID)
{
	occupied[newCreatureID >> 5] -= 1 << (newCreatureID & 31);//TODO make multicall-safe
}


void Creature::setUnoccupied(ID oldCreatureID)
{
	occupied[oldCreatureID >> 5] += 1 << (oldCreatureID & 31);//TODO make multicall-safe
}

ID Creature::getUnoccupiedID()
{
	unsigned int size = maxAlive >> 5;
	unsigned int i;
	for (i = 0; i < size; i++)
	{
		if (occupied[i] != 0) break;//if all bits are 0, all spaces are occupied
	}
	unsigned int test = occupied[i];
	for (unsigned int j = 0; j < 32; j++)
	{
		if (test&(1 << j)) return j + i << 5;
	}
	return ~0;
}
