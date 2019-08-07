/*
 * LockFast.cpp
 *
 *  Created on:	Oct 2, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "LockFast.h"

LockFast::LockFast()
{
	// TODO Auto-generated constructor stub

}

bool LockFast::try_lock()
{
	return m.try_lock();
}

void LockFast::lock()
{
	m.lock();
}

void LockFast::unlock()
{
	m.unlock();
}

LockFast::~LockFast()
{
	// TODO Auto-generated destructor stub
}

