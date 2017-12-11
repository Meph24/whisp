/*
 * EVMain.cpp
 *
 *  Created on:	Nov 23, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "EVMain.h"

EV_Main::EV_Main()
{
	w=new EV_World(64,64,3);
	w->randomInit();
}

EV_Main::~EV_Main()
{
	delete w;
}

