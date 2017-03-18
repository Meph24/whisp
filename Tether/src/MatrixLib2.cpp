#include "MatrixLib2.h"
#include <cmath>

MatrixLib2::MatrixLib2(uint8_t stacksize)
{
	matStack = new float[stacksize * 16];
	loadIdentity();
}


MatrixLib2::~MatrixLib2()
{
	delete[] matStack;
}



void MatrixLib2::loadIdentity()
{
	for (int i = 0; i < 16; i++)
	{
		curMatrix[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}
}

void MatrixLib2::multMatrix(float * mat)
{
	float temp[16];
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
	/*
	for (int x = 0; x < 4; x++)
	{
	for (int y = 0; y < 4; y++)
	{
	float sum = 0;
	for (int z = 0; z < 4; z++)
	{
	sum += curMatrix[y * 4 + z] * mat[z * 4 + x];
	}
	temp[x + y * 4] = sum;
	}
	}
	*/
	for (int i = 0; i < 16; i++)
	{
		curMatrix[i] = temp[i];
	}
}

void MatrixLib2::rotatef(float angle, float x, float y, float z)
{
	float angle_rad = angle * (TAU / 360.0f);//TODO change + change hard coded angles

	float c = cos(angle_rad);
	float s = sin(angle_rad);
	float v = 1 - c;//versinus

	//TODO normalize x,y,z

	float m[16] = {
		c + x*x*v, y*x*v + z*s, z*x*v - y*s, 0,
		x*y*v - z*s, c + y*y*v, z*y*v + x*s, 0,
		x*z*v + y*s, y*z*v - x*s, z*z*v + c, 0,
		0, 0, 0, 1
	};
	multMatrix(m);
}
void MatrixLib2::translatef(float x, float y, float z)
{
	float mat[16];
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
	/*float mat[16];
	for (int i = 0; i < 16; i++)
	{
	mat[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}
	mat[0] = x;
	mat[5] = y;
	mat[10] = z;
	multMatrix(mat);*/
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
	float * stackPtr = matStack + 16 * stack;
	for (int i = 0; i < 16; i++)
	{
		stackPtr[i] = curMatrix[i];
	}
	stack++;
}

void MatrixLib2::popMatrix()
{
	stack--;
	float * stackPtr = matStack + 16 * stack;
	for (int i = 0; i < 16; i++)
	{
		curMatrix[i] = stackPtr[i];
	}
}

void MatrixLib2::getMatrix(float * targetMem)
{
	for (int i = 0; i < 16; i++)
	{
		targetMem[i] = curMatrix[i];
	}
}

bool MatrixLib2::invertMatrix(float m[16], float invOut[16])
{
	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}


MatrixLib2::vec3 MatrixLib2::multWith(float * mat, vec3 * inp)
{
	vec3 ret;
	ret.x = mat[0] * inp->x + mat[4] * inp->y + mat[8] * inp->z + mat[12];
	ret.y = mat[1] * inp->x + mat[5] * inp->y + mat[9] * inp->z + mat[13];
	ret.z = mat[2] * inp->x + mat[6] * inp->y + mat[10] * inp->z + mat[14];
	return ret;
}
