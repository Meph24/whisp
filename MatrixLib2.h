#pragma once
#include <cstdint>
#include <cmath>

//TODO guard
#define TAU 6.283185307179586f

class MatrixLib2
{
	float * matStack;
	uint8_t stack = 0;

public:

	typedef struct
	{
		float x, y, z;
	} vec3;

	float curMatrix[16];

	void loadIdentity();
	void rotatef(float angle, float x, float y, float z);
	void translatef(float x, float y, float z);
	void scalef(float x, float y, float z);
	void pushMatrix();
	void popMatrix();
	void multMatrix(float * mat);
	void getMatrix(float * targetMem);
	void printMatrix();
	bool invertMatrix(float in[16], float out[16]);
	vec3 multWith(float * mat, vec3 * inp);
	MatrixLib2(uint8_t stacksize);
	~MatrixLib2();
};

