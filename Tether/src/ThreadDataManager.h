/*
 * ThreadDataManager.h
 *
 *  Created on:	Oct 8, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_THREADDATAMANAGER_H_
#define SRC_THREADDATAMANAGER_H_

#include "InteractionManager.h"
#include <vector>

class ThreadDataManager
{
	unsigned int threadCount;
	std::vector<InteractionManager *> ** interManVecs=0;
public:
	void reset(unsigned int newThreadCount);//do NOT call while ANY of the data is in use!!!

	std::vector<InteractionManager *> * getInteractionManVec(unsigned int threadID);
	ThreadDataManager(unsigned int threadCount=1);
	~ThreadDataManager();
};

#endif /* SRC_THREADDATAMANAGER_H_ */
