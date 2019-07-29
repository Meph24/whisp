/*
 * DebugScreen.cpp
 *
 *  Created on:	Mar 16, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "DebugScreen.h"

DebugScreen::DebugScreen(PerformanceMeter * p,Graphics2D ** gr):
g(gr),pm(p),textSize(0.1f)
{

}

DebugScreen::~DebugScreen()
{
	// TODO Auto-generated destructor stub
}

#include <sstream>
int DebugScreen::draw(flt x,flt y,flt z,int lineOffset)//TODO drawable + parameter setter function
{
	if(!pm) return lineOffset;
	int num=pm->getStepCount();
	int i;
	for(i=-1;i<num;i++)
	{
		std::string s=pm->getInfo(i,FLAG_RECENTAVG+FLAG_SPIKES+FLAG_NOW);
		g[0]->drawString(s.c_str(),s.length(),-1.4f,0.8f-(lineOffset-1)*textSize,textSize);
		lineOffset++;
	}
	std::ostringstream myString;
	myString<<"x = "<<x<<" | y = "<<y<<" | z = "<<z;
	std::string s=myString.str();
	g[0]->drawString(s.c_str(),s.length(),-1.4f,0.8f-(lineOffset-1)*textSize,textSize);
	lineOffset++;
	return lineOffset;
}

