#include "MatrixLib2.h"
#include <cmath>

MatrixLib2::MatrixLib2()
{
	loadIdentity();
}


MatrixLib2::~MatrixLib2()
{
}



void MatrixLib2::loadIdentity()
{
	for (int i = 0; i < 16; i++)
	{
		curMatrix[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}
}

void MatrixLib2::multMatrix(mat4 mat)
{
	mat4 temp;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			float sum = 0;
			for (int z = 0; z < 4; z++)
			{
				sum += curMatrix[y + z * 4] * mat[z + x * 4];
			}
			temp[x * 4 + y] = sum;
		}
	}
	curMatrix=temp;
}

void MatrixLib2::rotatef(float angle, float x, float y, float z)
{
	float angle_rad = angle * (TAU / 360.0f);//TODO change + change hard coded angles

	float c = cos(angle_rad);
	float s = sin(angle_rad);
	float v = 1 - c;//versinus

	//TODO normalize x,y,z

	mat4 m = {
		c + x*x*v, y*x*v + z*s, z*x*v - y*s, 0,
		x*y*v - z*s, c + y*y*v, z*y*v + x*s, 0,
		x*z*v + y*s, y*z*v - x*s, z*z*v + c, 0,
		0, 0, 0, 1
	};
	multMatrix(m);
}
void MatrixLib2::translatef(float x, float y, float z)
{
	mat4 mat;
	//TODO optimize: directly apply to curMatrix
	for (int i = 0; i < 16; i++)
	{
		mat[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
	multMatrix(mat);
}
void MatrixLib2::scalef(float x, float y, float z)
{
	for (int i = 0; i < 4; i++)
		curMatrix[i] *= x;

	for (int i = 0; i < 4; i++)
		curMatrix[i + 4] *= y;

	for (int i = 0; i < 4; i++)
		curMatrix[i + 8] *= z;

	//TODO check optimization
}
#include <iostream>
#include <iomanip>
void MatrixLib2::printMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		std::cout << std::fixed << std::setprecision(3) << curMatrix[i] << "		" << curMatrix[i + 4] << "		" << curMatrix[i + 8] << "		" << curMatrix[i + 12] << std::endl << std::endl;
	}
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
}

void MatrixLib2::pushMatrix()
{
	matStack.push_back(curMatrix);
}

void MatrixLib2::popMatrix()
{
	curMatrix=matStack.back();
	matStack.pop_back();
}

bool MatrixLib2::invertMatrix(mat4 in, mat4& out)
{
	mat4 inv;
	flt det;
	int i;

	inv[0] = in[5] * in[10] * in[15] -
		in[5] * in[11] * in[14] -
		in[9] * in[6] * in[15] +
		in[9] * in[7] * in[14] +
		in[13] * in[6] * in[11] -
		in[13] * in[7] * in[10];

	inv[4] = -in[4] * in[10] * in[15] +
		in[4] * in[11] * in[14] +
		in[8] * in[6] * in[15] -
		in[8] * in[7] * in[14] -
		in[12] * in[6] * in[11] +
		in[12] * in[7] * in[10];

	inv[8] = in[4] * in[9] * in[15] -
		in[4] * in[11] * in[13] -
		in[8] * in[5] * in[15] +
		in[8] * in[7] * in[13] +
		in[12] * in[5] * in[11] -
		in[12] * in[7] * in[9];

	inv[12] = -in[4] * in[9] * in[14] +
		in[4] * in[10] * in[13] +
		in[8] * in[5] * in[14] -
		in[8] * in[6] * in[13] -
		in[12] * in[5] * in[10] +
		in[12] * in[6] * in[9];

	inv[1] = -in[1] * in[10] * in[15] +
		in[1] * in[11] * in[14] +
		in[9] * in[2] * in[15] -
		in[9] * in[3] * in[14] -
		in[13] * in[2] * in[11] +
		in[13] * in[3] * in[10];

	inv[5] = in[0] * in[10] * in[15] -
		in[0] * in[11] * in[14] -
		in[8] * in[2] * in[15] +
		in[8] * in[3] * in[14] +
		in[12] * in[2] * in[11] -
		in[12] * in[3] * in[10];

	inv[9] = -in[0] * in[9] * in[15] +
		in[0] * in[11] * in[13] +
		in[8] * in[1] * in[15] -
		in[8] * in[3] * in[13] -
		in[12] * in[1] * in[11] +
		in[12] * in[3] * in[9];

	inv[13] = in[0] * in[9] * in[14] -
		in[0] * in[10] * in[13] -
		in[8] * in[1] * in[14] +
		in[8] * in[2] * in[13] +
		in[12] * in[1] * in[10] -
		in[12] * in[2] * in[9];

	inv[2] = in[1] * in[6] * in[15] -
		in[1] * in[7] * in[14] -
		in[5] * in[2] * in[15] +
		in[5] * in[3] * in[14] +
		in[13] * in[2] * in[7] -
		in[13] * in[3] * in[6];

	inv[6] = -in[0] * in[6] * in[15] +
		in[0] * in[7] * in[14] +
		in[4] * in[2] * in[15] -
		in[4] * in[3] * in[14] -
		in[12] * in[2] * in[7] +
		in[12] * in[3] * in[6];

	inv[10] = in[0] * in[5] * in[15] -
		in[0] * in[7] * in[13] -
		in[4] * in[1] * in[15] +
		in[4] * in[3] * in[13] +
		in[12] * in[1] * in[7] -
		in[12] * in[3] * in[5];

	inv[14] = -in[0] * in[5] * in[14] +
		in[0] * in[6] * in[13] +
		in[4] * in[1] * in[14] -
		in[4] * in[2] * in[13] -
		in[12] * in[1] * in[6] +
		in[12] * in[2] * in[5];

	inv[3] = -in[1] * in[6] * in[11] +
		in[1] * in[7] * in[10] +
		in[5] * in[2] * in[11] -
		in[5] * in[3] * in[10] -
		in[9] * in[2] * in[7] +
		in[9] * in[3] * in[6];

	inv[7] = in[0] * in[6] * in[11] -
		in[0] * in[7] * in[10] -
		in[4] * in[2] * in[11] +
		in[4] * in[3] * in[10] +
		in[8] * in[2] * in[7] -
		in[8] * in[3] * in[6];

	inv[11] = -in[0] * in[5] * in[11] +
		in[0] * in[7] * in[9] +
		in[4] * in[1] * in[11] -
		in[4] * in[3] * in[9] -
		in[8] * in[1] * in[7] +
		in[8] * in[3] * in[5];

	inv[15] = in[0] * in[5] * in[10] -
		in[0] * in[6] * in[9] -
		in[4] * in[1] * in[10] +
		in[4] * in[2] * in[9] +
		in[8] * in[1] * in[6] -
		in[8] * in[2] * in[5];

	det = in[0] * inv[0] + in[1] * inv[4] + in[2] * inv[8] + in[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
		out[i] = inv[i] * det;

	return true;
}
