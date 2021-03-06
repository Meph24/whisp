/*
 * ITerrain.h
 *
 *  Created on:	29.11.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ITERRAIN_H_
#define SRC_ITERRAIN_H_

#include "Drawable.h"

class IWorld;

#include "Spacevec.h"
#include "IWorld.h"


class ITerrain//: public Drawable
{
	spacevec defaultGravity;
public:
	ITerrain(IWorld * world);
	virtual ~ITerrain();

	virtual void terrainCalcs(TickServiceProvider * tsp,spacevec playerPos);
	virtual spacevec getGravity(spacevec pos);

	virtual spacevec clip(spacevec pos,bool forceGround,IWorld& iw)=0;//If pos is below ground or forceGround, then return position on ground (movement along gravity vector)
	virtual bool hitsGround(spacevec startpoint,spacevec endpoint,IWorld& iw)=0;//true if any point in the given line is below ground
};

#endif /* SRC_ITERRAIN_H_ */
