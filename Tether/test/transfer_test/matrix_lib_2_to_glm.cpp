#include "../../src/CumulativeMat.hpp"

#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <algorithm>

using glm::vec3;

#define TAU (float)(6.283185307179586)

void MatrixLib2_loadIdentity(mat4& m)
{
	const float* in = glm::value_ptr(m);
	float curMatrix [16];
	std::copy(in, in+16, curMatrix);
	for (int i = 0; i < 16; i++)
	{
		curMatrix[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}
	m = glm::make_mat4x4(curMatrix);
}

void MatrixLib2_multMatrix(mat4& m1,const mat4& m2)
{
	const float* m1p = glm::value_ptr(m1);
	const float* m2p = glm::value_ptr(m2);
	float temp[16];
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			float sum = 0;
			for (int z = 0; z < 4; z++)
			{
				sum += m1p[y + z * 4] * m2p[z + x * 4];
			}
			temp[x * 4 + y] = sum;
		}
	}
	m1 = glm::make_mat4x4(temp);
}

void MatrixLib2_rotatef(mat4& m, float angle, float x, float y, float z)
{
	float angle_rad = angle * (TAU / 360.0f);//TODO change + change hard coded angles

	float c = cos(angle_rad);
	float s = sin(angle_rad);
	float v = 1 - c;//versinus

	//TODO normalize x,y,z

	mat4 mrot (
		c + x*x*v, y*x*v + z*s, z*x*v - y*s, 0.0f,
		x*y*v - z*s, c + y*y*v, z*y*v + x*s, 0.0f,
		x*z*v + y*s, y*z*v - x*s, z*z*v + c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	MatrixLib2_multMatrix(m, mrot);
}
void MatrixLib2_translatef(mat4& m, float x, float y, float z)
{
	float mattrans[16];
	//TODO optimize: directly apply to curMatrix
	for (int i = 0; i < 16; i++)
	{
		mattrans[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}
	mattrans[12] = x;
	mattrans[13] = y;
	mattrans[14] = z;

	MatrixLib2_multMatrix(m, glm::make_mat4x4(mattrans));
}

void MatrixLib2_scalef(mat4& m, float x, float y, float z)
{
	const float* mp = glm::value_ptr(m);
	float curMatrix[16];
	std::copy(mp, mp+16, curMatrix);
	for (int i = 0; i < 4; i++)
		curMatrix[i] *= x;

	for (int i = 0; i < 4; i++)
		curMatrix[i + 4] *= y;

	for (int i = 0; i < 4; i++)
		curMatrix[i + 8] *= z;

	m = glm::make_mat4x4(curMatrix);
}

#include <iostream>
#include <iomanip>

void MatrixLib2_printMatrix(mat4& m)
{
	float* vp = glm::value_ptr(m);
	for (int i = 0; i < 4; i++)
	{
		std::cout << std::fixed << std::setprecision(3) <<  vp[i] << " " << ((!(i % 4))? "\n" : "") << std::endl << std::endl;
	}
	std::cout << std::endl << std::endl;;
}

bool MatrixLib2_invertMatrix(mat4 inp, mat4& o)
{
	float in[16];
	float out[16];

	const float* vp = glm::value_ptr(inp);
	std::copy(vp, vp+16, in);

	float inv[16];
	float det;
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
	o = glm::make_mat4x4(out);

	return true;
}



struct test_Transfer_CumulativeMat : public ::testing::Test
{
	mat4 m1, m2;
public:
	test_Transfer_CumulativeMat()
		:
			m1(1.0f),
			m2(2.0f, 1.0f, 3.0f, 4.0f,
			   1.0f, 5.0f, 0.5f, 1.0f,
			   1.5f, -2.0f, 1.0f, 1.0f,
			   10.0f,2.0f, 3.0f, 4.0f)
	{
	}
	~test_Transfer_CumulativeMat()
	{
	}
};

TEST_F(test_Transfer_CumulativeMat, loadIdentity)
{
	MatrixLib2_loadIdentity(m2);
	ASSERT_EQ(m1, m2);
}

TEST_F(test_Transfer_CumulativeMat, mult)
{
	m1 = mat4(2.0f, 3.0f, 4.0f, 1.5f,
			  1.0f, 1.0f, 1.0f, 1.0f,
			  0,0,0,0,
			  10, 8, 6.5, 8);

	mat4 ret1, ret2;
	ret1 = m1;
	MatrixLib2_multMatrix(ret1, m2);
	
	ret2 = m1 * m2;

	ASSERT_EQ(ret1, ret2);
}

TEST_F(test_Transfer_CumulativeMat, rotate)
{
	mat4 ret1, ret2;

	ret1 = m2;

	vec3 norm (1,0,1);
	norm = glm::normalize(norm);
	MatrixLib2_rotatef(ret1, 90, norm.x, norm.y, norm.z);

	ret2 = m2 * glm::rotate(glm::radians(90.0f), vec3(1, 0, 1));

	ASSERT_EQ(ret1, ret2) << glm::to_string(ret1) << '\n' << glm::to_string(ret2);
}

TEST_F(test_Transfer_CumulativeMat, transfer)
{
	mat4 ret1, ret2;
	ret1 = m2;

	MatrixLib2_translatef(ret1, 2, 0, 2);

	ret2 = glm::translate(m2, vec3(2, 0, 2));
	ASSERT_EQ(ret1, ret2);
}

TEST_F(test_Transfer_CumulativeMat, scale)
{
	mat4 ret1, ret2;
	ret1 = m2;

	MatrixLib2_scalef(ret1, 2, 0, 2);

	ret2 = glm::scale(m2, vec3(2, 0, 2));
	ASSERT_EQ(ret1, ret2);
}

TEST_F(test_Transfer_CumulativeMat, inverse)
{
	mat4 ret1, ret2;
	
	MatrixLib2_invertMatrix(m2, ret1);

	ret2 = glm::inverse(m2);

	ASSERT_EQ(ret1, ret2);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
