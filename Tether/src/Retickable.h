/*
 * Retickable.h
 *
 *  Created on:	12.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_RETICKABLE_H_
#define SRC_RETICKABLE_H_

class TickServiceProvider;

class Retickable
{
public:
	virtual void retick(TickServiceProvider * tsp);

	Retickable();
	virtual ~Retickable();
};

#endif /* SRC_RETICKABLE_H_ */
