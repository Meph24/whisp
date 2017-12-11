/*
 * ICamera2D.h
 *
 *  Created on:	Sep 5, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ICAMERA2D_H_
#define SRC_ICAMERA2D_H_

#include "ICamera.h"

class ICamera2D: public ICamera
{
public:
	float gamma;

	virtual void apply()=0;

	ICamera2D();
	virtual ~ICamera2D();
};

#endif /* SRC_ICAMERA2D_H_ */
