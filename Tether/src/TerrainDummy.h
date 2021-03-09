/*
 * TerrainDummy.h
 *
 *  Created on:	Dec 17, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_TERRAINDUMMY_H_
#define SRC_TERRAINDUMMY_H_

#include "ITerrain.h"

class IWorld;

#include "Spacevec.h"

class TerrainDummy: public ITerrain
{
public:
	spacelen height;
	virtual spacevec clip(spacevec pos,bool forceGround,IWorld& iw);//If pos is below ground or forceGround, then return position on ground (movement along gravity vector)
	virtual bool hitsGround(spacevec startpoint,spacevec endpoint,IWorld& iw);//true if any point in the given line is below ground

	TerrainDummy(IWorld * iw,spacelen Height);
	virtual ~TerrainDummy();
};

#endif /* SRC_TERRAINDUMMY_H_ */
