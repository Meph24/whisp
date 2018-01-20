/*
 * Tickable.h
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_TICKABLE_H_
#define SRC_TICKABLE_H_

class Tickable
{
public:
	virtual void tick(float time);
	Tickable();
	~Tickable();
};

#endif /* SRC_TICKABLE_H_ */
