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

//#include <iostream>
void DebugScreen::draw()
{
	if(!pm) return;
	int num=pm->getStepCount();
	for(int i=-1;i<(num-1);i++)
	{
		std::string s=pm->getInfo(i,FLAG_RECENTAVG+FLAG_SPIKES+FLAG_NOW);
		g[0]->drawString(s.c_str(),s.length(),-1.4f,0.8f-i*textSize,textSize);
		//std::cout<<s<<std::endl;
	}
}

