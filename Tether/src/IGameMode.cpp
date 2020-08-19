/*
 * IGameMode.cpp
 *
 *  Created on:	Dec 11, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "IGameMode.h"

IGameMode::IGameMode(const WallClock& reference_clock)
	: clock(reference_clock, /* rate */ 0.0f, 1ms, 40us)
	, tm(1,1000,40)//TODO 20/20)
{}
