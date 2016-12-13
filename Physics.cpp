#include "Physics.h"


Physics::Physics(unsigned int elementCount)
{
	n = elementCount;
	front = new physicsBuffer;
	back = new physicsBuffer;

	float * ptr = new float[n * 8];
	front->x1 = ptr;
	front->x2 = ptr + n;
	front->y1 = ptr + 2 * n;
	front->y2 = ptr + 3 * n;
	front->elasticity = ptr + 4 * n;
	front->vx = ptr + 5 * n;
	front->vy = ptr + 6 * n;
	front->m = ptr + 7 * n;
	front->t = new timestamp[n];

	ptr = new float[n * 8];
	back->x1 = ptr;
	back->x2 = ptr + n;
	back->y1 = ptr + 2 * n;
	back->y2 = ptr + 3 * n;
	back->elasticity = ptr + 4 * n;
	back->vx = ptr + 5 * n;
	back->vy = ptr + 6 * n;
	back->m = ptr + 7 * n;
	back->t = new timestamp[n];
}

Physics::~Physics()
{
	delete[] front->x1;
	delete[] front->t;
	delete front;
	delete[] back->x1;
	delete[] back->t;
	delete back;
}

void Physics::initBuffer(physicsBuffer * * buffer)
{
	*buffer = new physicsBuffer;
	physicsBuffer * b = *buffer;
	b->t = new timestamp[n];
	//float * ptr = new float[n * 8];

}
void Physics::deleteBuffer(physicsBuffer * buffer)
{

}

void Physics::bufSwap()
{
	physicsBuffer * temp=front;
	front = back;
	back = temp;
}

void Physics::applyCollisionX(index a, index b) //TODO optimize
{
	float m1 = front->m[a];
	float m2 = front->m[b];
	float v1 = front->vx[a];
	float v2 = front->vx[b];
	float elast = (front->elasticity[a] + front->elasticity[b])*0.5f;
	float r1 = m1 * v1 + m2 * v2;
	float r2 = 1.0f / (m1 + m2);
	back->vx[a] = (r1 - m2*(v1 - v2)*elast)*r2;
	back->vx[b] = (r1 - m1*(v2 - v1)*elast)*r2;
}
void Physics::applyCollisionY(index a, index b) //TODO optimize
{
	float m1 = front->m[a];
	float m2 = front->m[b];
	float v1 = front->vy[a];
	float v2 = front->vy[b];
	float elast = (front->elasticity[a] + front->elasticity[b])*0.5f;
	float r1 = m1 * v1 + m2 * v2;
	float r2 = 1.0f / (m1 + m2);
	back->vy[a] = (r1 - m2*(v1 - v2)*elast)*r2;
	back->vy[b] = (r1 - m1*(v2 - v1)*elast)*r2;
}
void Physics::tryCollision(index a, index b)
{

}
