/*
 * DebugScreen.h
 *
 *  Created on:	Mar 16, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_DEBUGSCREEN_H_
#define SRC_DEBUGSCREEN_H_

#include "MathStuff.h"

class Graphics2D;
class PerformanceMeter;

class DebugScreen
{
	Graphics2D ** g;
	PerformanceMeter * pm;
public:
	float textSize;
	DebugScreen(PerformanceMeter * p,Graphics2D ** gr);
	~DebugScreen();


	int draw(float x,float y,float z,int lineOffset);
};

#endif /* SRC_DEBUGSCREEN_H_ */
