#pragma once
#include <cstdint>

//TODO guard
#define TAU 6.283185307179586f

class MatrixLib
{
	float * matStack;
	uint8_t stack=0;

public:
	float curMatrix[16];

	void loadIdentity();
	void rotatef(float angle,float x,float y,float z);
	void translatef(float x,float y,float z);
	void scalef(float x, float y, float z);
	void pushMatrix();
	void popMatrix();
	void multMatrix(float * mat);
	void printMatrix();
	MatrixLib(uint8_t stacksize);
	~MatrixLib();
};

