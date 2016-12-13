#pragma once
#include "Helper.h"
class Physics
{
	typedef struct
	{
		float * x1;
		float * x2;
		float * y1;
		float * y2;
		float * elasticity;
		float * vx;
		float * vy;
		float * m;
		timestamp * t;

	} physicsBuffer;

	void initBuffer(physicsBuffer * * buffer);
	void deleteBuffer(physicsBuffer * buffer);
	void bufSwap();
	void applyCollisionX(index a, index b);
	void applyCollisionY(index a, index b);
	void tryCollision(index a, index b);

	physicsBuffer * front;
	physicsBuffer * back;
	unsigned int n;

public:

	void calcPhysics();

	Physics(unsigned int elementCount);
	~Physics();
};

