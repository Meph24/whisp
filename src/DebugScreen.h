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

class DebugScreen
{
	Graphics2D g;
public:
	float textSize;
	DebugScreen();
	~DebugScreen();



	void draw();
};

#endif /* SRC_DEBUGSCREEN_H_ */
