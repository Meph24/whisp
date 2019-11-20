/*
 * WorldDefault.h
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_WORLDDEFAULT_H_
#define SRC_WORLDDEFAULT_H_

#include "IWorld.h"

#include <vector>

class WorldDefault: public IWorld
{
	std::vector<Entity *> entities;
public:
	WorldDefault();
	virtual ~WorldDefault();

	virtual void requestEntitySpawn(Entity * e);
};

#endif /* SRC_WORLDDEFAULT_H_ */
