#pragma once
#include "EntityProjectile.h"
#include "Pushable.h"
#include "MatrixLib2.h"
class Zombie_Physics//TODO replace and remove
{

public:
	ChunkManager * cm=0;

	typedef struct
	{
		int projectileIndex;//-1 if not hit
		float time;//0=begin of frame 1=end of frame
	} hit;

	EntityProjectile ** projectiles;
	int* pCount;

	Zombie_Physics(int maxObjects);
	~Zombie_Physics();

	hit testHitbox(MatrixLib2 * ml,float xFrom, float xTo, float yFrom, float yTo, float zFrom, float zTo,spacevec relPos);
	
};

