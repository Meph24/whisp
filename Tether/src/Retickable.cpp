/*
 * Retickable.cpp
 *
 *  Created on:	12.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "Retickable.h"

Retickable::Retickable()
{
	// TODO Auto-generated constructor stub

}
#include "WarnErrReporter.h"
void Retickable::retick(TickServiceProvider* tsp)
{
	WarnErrReporter::noOverrideErr("requested retick without overriding Retickable::retick(...)");
}

Retickable::~Retickable()
{
	// TODO Auto-generated destructor stub
}

