#pragma once
#include "MathStuff.h"

class mat3
{
public:
	flt mat[9];

	mat3();
	mat3(flt,flt,flt,flt,flt,flt,flt,flt,flt);
	~mat3();

	//inline
	flt operator[](int index);
};

inline mat3 operator_mat3(flt one);

//TODO
