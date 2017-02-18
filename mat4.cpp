#include "mat4.h"


mat4::mat4()
{

}

mat4::mat4(flt mat0,flt mat1,flt mat2,flt mat3,flt mat4,flt mat5,flt mat6,flt mat7,flt mat8,flt mat9,flt mat10,flt mat11,flt mat12,flt mat13,flt mat14,flt mat15):
mat{ mat0, mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10, mat11, mat12, mat13, mat14, mat15}
{

}

mat4::~mat4()
{

}


inline mat4 operator_mat4(flt one)
{
	return mat4
	{
		one, 0.0f, 0.0f, 0.0f,
		0.0f, one, 0.0f, 0.0f,
		0.0f, 0.0f, one, 0.0f,
		0.0f, 0.0f, 0.0f, one
	};
}

inline flt mat4::operator[](int index)
{
	return mat[index];
}

