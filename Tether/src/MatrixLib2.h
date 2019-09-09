#pragma once

#include <vector>

#include "MatrixLib.h"

class MatrixLib2
{
	std::vector<mat4> matStack;

public:
	mat4 curMatrix;

	void loadIdentity();
	void rotatef(float angle, float x, float y, float z);
	void translatef(float x, float y, float z);
	void scalef(float x, float y, float z);
	void pushMatrix();
	void popMatrix();
	void multMatrix(mat4 mat);
	void printMatrix();
	bool invertMatrix(mat4 m, mat4& out);
	MatrixLib2();
	~MatrixLib2();
};

