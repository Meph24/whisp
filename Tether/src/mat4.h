#pragma once

#include "MathStuff.h"

class mat4
{
public:
	flt mat[16];

	mat4();
	mat4(flt,flt,flt,flt,flt,flt,flt,flt,flt,flt,flt,flt,flt,flt,flt,flt);
	~mat4();

	//inline
	flt operator[](int index);
};

//inline
mat4 operator_mat4(flt one);


//TODO
