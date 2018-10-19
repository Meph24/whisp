/*
 * LockFast.h
 *
 *  Created on:	Oct 2, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_LOCKFAST_H_
#define SRC_LOCKFAST_H_

#include <mutex>

//this lock tries to create least delays as possible.
//TODO internals should be updated when better suitable implementation is found  (aiming for spinlock)
//meanwhile it uses the first example i found online
class LockFast
{
	std::mutex m;
public:
	bool try_lock();
	void lock();
	void unlock();

	LockFast();
	~LockFast();
};

#endif /* SRC_LOCKFAST_H_ */
