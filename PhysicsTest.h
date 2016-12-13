#pragma once
class PhysicsTest
{
private:
	float m1,m2,v1,v2;
	float elasticity=0.5f;
	float v1new, v2new;
public:
	void calcCollision();
	void evaluateCalculation();
	PhysicsTest();
	~PhysicsTest();
};