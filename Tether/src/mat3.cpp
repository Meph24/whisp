#include "mat3.h"


mat3::mat3()
{

}


mat3::~mat3()
{

}


mat3::mat3(flt mat0,flt mat1,flt mat2,flt mat3,flt mat4,flt mat5,flt mat6,flt mat7,flt mat8):
mat{ mat0, mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8}
{

}

inline mat3 operator_mat3(flt one)
{
	return mat3
	{
		one, 0.0f, 0.0f,
		0.0f, one, 0.0f,
		0.0f, 0.0f, one
	};
}

//inline
flt mat3::operator[](int index)
{
	return mat[index];
}
