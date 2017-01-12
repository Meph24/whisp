#include "NoiseGen2D.h"



NoiseGen2D::NoiseGen2D(index x, index y)
{
	sizex = x;
	sizey = y;
	size = x * y;
	noise = new float[size];
}

NoiseGen2D::NoiseGen2D(index x, index y,float * target)
{
	sizex = x;
	sizey = y;
	size = x * y;
	noise = target;
}


NoiseGen2D::~NoiseGen2D()
{

}
