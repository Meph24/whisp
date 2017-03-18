/*
 * DebugScreen.cpp
 *
 *  Created on:	Mar 16, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "DebugScreen.h"

DebugScreen::DebugScreen():
g(16),textSize(0.1f)
{

}

DebugScreen::~DebugScreen()
{
	// TODO Auto-generated destructor stub
}


void DebugScreen::draw()
{
	g.drawString("debugTextText",13,0.9f,0.9f,textSize);
}
