/*
 * InteractionManager.h
 *
 *  Created on:	19.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_INTERACTIONMANAGER_H_
#define SRC_INTERACTIONMANAGER_H_
#include "Pushable.h"
#include "InteractionGroup1.h"
#include "InteractionGroup2.h"
class InteractionManager
{
public:
	InteractionGroup1<Pushable> push;

	void resetAll();

	InteractionManager();
	~InteractionManager();
};

#endif /* SRC_INTERACTIONMANAGER_H_ */
