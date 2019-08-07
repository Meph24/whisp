/*
 * DebugScreen.h
 *
 *  Created on:	Mar 16, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_DEBUGSCREEN_H_
#define SRC_DEBUGSCREEN_H_


#include "Graphics2D.h"
#include "PerformanceMeter.h"

class DebugScreen
{
	Graphics2D ** g;
	PerformanceMeter * pm;
public:
	float textSize;
	DebugScreen(PerformanceMeter * p,Graphics2D ** gr);
	~DebugScreen();


	int draw(flt x,flt y,flt z,int lineOffset);
};

#endif /* SRC_DEBUGSCREEN_H_ */
