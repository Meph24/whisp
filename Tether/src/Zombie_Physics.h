#pragma once
#include "EntityProjectile.h"
#include "MatrixLib2.h"
class Zombie_Physics
{
	float * xPush;
	float * zPush;
	float * xObj;
	float * zObj;
	float * force;
	float * radius;
	int * pos;
	int curMax;
	int totalMax;


	//MatrixLib2 ml;

public:
	ChunkManager * cm=0;

	typedef struct
	{
		int projectileIndex;//-1 if not hit
		float time;//0=begin of frame 1=end of frame
	} hit;

	EntityProjectile ** projectiles;
	int* pCount;

	typedef struct
	{
		float x, z;
	} motion;
	Zombie_Physics(int maxObjects);
	~Zombie_Physics();
	bool registerObject(int ID, float force, float xObj, float zObj, float radius);
	motion getMotion(int ID,float time);
	void doPushPhysics();

	hit testHitbox(MatrixLib2 * ml,float xFrom, float xTo, float yFrom, float yTo, float zFrom, float zTo);
	
};

