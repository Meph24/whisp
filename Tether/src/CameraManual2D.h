/*
 * CameraManual2D.h
 *
 *  Created on:	Sep 5, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_CAMERAMANUAL2D_H_
#define SRC_CAMERAMANUAL2D_H_

#include "ICamera2D.h"

class CameraManual2D: public ICamera2D
{
public:

	void apply();
	CameraManual2D(int resX,int resY, Eye& eye);
	~CameraManual2D() = default;
};

#endif /* SRC_CAMERAMANUAL2D_H_ */
