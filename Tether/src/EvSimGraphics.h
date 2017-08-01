/*
 * EvSimGraphics.h
 *
 *  Created on:	Jun 22, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_EVSIMGRAPHICS_H_
#define SRC_EVSIMGRAPHICS_H_

#include "MatrixLib.h"
#include "ICamera.h"

class EvSimGraphics
{
public:
	void drawNode(vec3 pos,vec3 vel);

	void startFrame(float secondsSinceTick,ICamera& cam);
	void endFrame();

	EvSimGraphics();
	~EvSimGraphics();
};

#endif /* SRC_EVSIMGRAPHICS_H_ */
