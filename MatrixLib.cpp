#include "MatrixLib.h"
#include <cmath>

MatrixLib::MatrixLib(uint8_t stacksize)
{
	matStack = new float[stacksize * 16];
	loadIdentity();
}


MatrixLib::~MatrixLib()
{
	delete[] matStack;
}



void MatrixLib::loadIdentity()
{
	for (int i = 0; i < 16; i++)
	{
		curMatrix[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}
}

void MatrixLib::multMatrix(float * mat)
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

void MatrixLib::rotatef(float angle, float x, float y, float z)
{
	float angle_rad = angle * (TAU/360.0f);//TODO change + change hard coded angles

	float c = cos(angle_rad);
	float s = sin(angle_rad);
	float v = 1 - c;//versinus

	//TODO normalize x,y,z

	float m[16] = {
		c + x*x*v,		y*x*v + z*s,		z*x*v - y*s,		0,
		x*y*v - z*s,	c + y*y*v,			z*y*v + x*s,		0,
		x*z*v + y*s,	y*z*v - x*s,		z*z*v + c,			0,
		0,				0,					0,					1
	};
	multMatrix(m);
}
void MatrixLib::translatef(float x, float y, float z)
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
void MatrixLib::scalef(float x, float y, float z)
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
		curMatrix[i+4] *= y;

	for (int i = 0; i < 4; i++)
		curMatrix[i + 8] *= z;
	//TODO check optimization


}
#include <iostream>
#include <iomanip>
void MatrixLib::printMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		std::cout << std::fixed << std::setprecision(3) << curMatrix[i] << "		" << curMatrix[i + 4] << "		" << curMatrix[i + 8] << "		" << curMatrix[i + 12] << std::endl << std::endl;
	}
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
}
void MatrixLib::pushMatrix()
{
	float * stackPtr = matStack + 16 * stack;
	for (int i = 0; i < 16; i++)
	{
		stackPtr[i] = curMatrix[i];
	}
	stack++;
}

void MatrixLib::popMatrix()
{
	stack--;
	float * stackPtr = matStack + 16 * stack;
	for (int i = 0; i < 16; i++)
	{
		curMatrix[i] = stackPtr[i];
	}
}

void MatrixLib::getMatrix(float * targetMem)
{
	for (int i = 0; i < 16; i++)
	{
		targetMem[i] = curMatrix[i];
	}
}