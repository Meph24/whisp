/*
 * TerrainDummy.cpp
 *
 *  Created on:	Dec 17, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "TerrainDummy.h"

TerrainDummy::TerrainDummy(IWorld * iw,spacelen Height):
ITerrain(iw),height(Height)
{
}

spacevec TerrainDummy::clip(spacevec pos, bool forceGround)
{
	if(forceGround || hitsGround(pos,pos))
	{
		pos.y=height;
	}
	return pos;
}

bool TerrainDummy::hitsGround(spacevec startpoint, spacevec endpoint)
{
	if(startpoint.y<height) return true;
	if(endpoint.y<height) return true;
	return false;
}

TerrainDummy::~TerrainDummy()
{
}

