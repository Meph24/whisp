#include "PhysicsTest.h"

#include <cmath>
#include <iostream>

PhysicsTest::PhysicsTest()
{
	m1 = 20;
	m2 = 1;
	v1 = 0.1f;
	v2 = 1;
}

void PhysicsTest::calcCollision()
{
	//https://de.wikipedia.org/wiki/Sto%C3%9F_%28Physik%29
	float r1 = m1*v1 + m2*v2;
	float r2 = 1.0f / (m1 + m2);
	v1new = (r1 - m2*(v1 - v2)*elasticity)*r2;
	v2new = (r1 - m1*(v2 - v1)*elasticity)*r2;
}

void PhysicsTest::evaluateCalculation()
{
	double imp1 = (v1*m1 + v2*m2);
	double imp2 = (v1new*m1 + v2new*m2);
	double impulseError;
	if (imp1 == 0)
	{
		std::cout << "impulse: expected 0, got " << imp2 << std::endl;
	}
	else
	{
		impulseError = imp2 / imp1;
		std::cout << "impulse: " << impulseError << std::endl;
	}

	double en1 = (v1*v1*m1 + v2*v2*m2);
	double en2 = (v1new*v1new*m1 + v2new*v2new*m2);
	double energyError;
	if (en1 == 0)
	{
		if (en2 == 0) energyError = 1;
		else energyError = 0;
	}
	else energyError = en2 / en1;
	std::cout << "energy: " << energyError << std::endl;
	float before = v1 - v2;
	float after = v1new - v2new;
	std::cout << "relative speed before: " << before << "; after: " << after << "; elasticity: "<<elasticity << std::endl;
	if (before<0 && after<0) std::cout << "wrong direction" << std::endl;
	else if (before>0 && after>0) std::cout << "wrong direction" << std::endl;
}

PhysicsTest::~PhysicsTest()
{

}
