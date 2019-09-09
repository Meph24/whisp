/*
 * MatrixLib.h
 *
 *  Created on:	14.02.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_MATRIXLIB_H_
#define SRC_MATRIXLIB_H_

#include "vec3.h"
#include "mat4.h"


vec3 operator*(mat4 a, vec3 b);

vec3 operator*(vec3 b, mat4 a);



//TODO

#endif /*SRC_MATRIXLIB_H*/
