#pragma once

#include "RandomDefines.h"
#include "ArrayDefines.h"


class NoiseGen2D
{
	index sizex;
	index sizey;
	index size;
public:
	float * noise;

	NoiseGen2D(index x, index y);
	NoiseGen2D(index x, index y,float * target);
	~NoiseGen2D();

	void generate(int octaves, int octaveOffset);

};

