/*
 * Timestamp.cpp
 *
 *  Created on:	28.04.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "Timestamp.h"

float Timestamp::operator -(Timestamp other)
{
	return (time-other.time)*0.000001f;
}

Timestamp::Timestamp(int microseconds):
		time(microseconds)
{
}

Timestamp::Timestamp()
{}
