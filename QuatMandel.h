/*
 * QuatMandel.h
 *
 *  Created on:	17.02.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_QUATMANDEL_H_
#define SRC_QUATMANDEL_H_

#include "TexParamSet.h"
#include "TextureDynamic2Dcpu.h"
class QuatMandel
{
	int * pixelData;
	int * mandelbrot;
	TexParamSet * tps;
public:
	int resolution;
	int iter;
	TextureDynamic2Dcpu ** texturesX;
	TextureDynamic2Dcpu ** texturesY;
	TextureDynamic2Dcpu ** texturesZ;
	QuatMandel(unsigned short size,int iterations);
	~QuatMandel();
	void initTextures();
	void update();
	void draw();
	void calc();
};

#endif /* SRC_QUATMANDEL_H_ */
