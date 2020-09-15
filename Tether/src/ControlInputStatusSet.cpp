#include "ControlInputStatusSet.hpp"

#include"WarnErrReporter.h"

ControlInputStatusSet::ControlInputStatusSet()
{
	fill(0.0f);
}

float ControlInputStatusSet::status(size_type id)
{ return at(id); }

void ControlInputStatusSet::setConditionTrue(size_type id)//TODO use proper int kind as ID
{
	if( id > size() )
	{
		WarnErrReporter::invalidValueErr("setConditionTrue was called with out of bounds ID value, ignoring this call...");
		return;
	}
	at(id)=1;
}

void ControlInputStatusSet::setConditionFalse(size_type id)
{
	if( id > size() )
	{
		WarnErrReporter::invalidValueErr("setConditionFalse was called with out of bounds ID value, ignoring this call...");
		return;
	}
	at(id)=0;
}

void ControlInputStatusSet::setConditionORedTrue(size_type id)
{
	if( id > size() )
	{
		WarnErrReporter::invalidValueErr("setConditionORedTrue was called with out of bounds ID value, ignoring this call...");
		return;
	}
	at(id)++;
}

void ControlInputStatusSet::setConditionORedFalse(size_type id)
{
	if( id > size() )
	{
		WarnErrReporter::invalidValueErr("setConditionORedFalse was called with out of bounds ID value, ignoring this call...");
		return;
	}
	at(id)--;
}

void ControlInputStatusSet::toggleCondition(size_type id)
{
	if( id>size() )
	{
		WarnErrReporter::invalidValueErr("toggleCondition was called with out of bounds ID value, ignoring this call...");
		return;
	}
	at(id) = !at(id);
}

float ControlInputStatusSet::getStatusAndReset(int index,float resetTo)
{
	float ret = at(index);
	at(index)=resetTo;
	return ret;
}

void SimulationInputStatusSet::toggleCondition(bool& b) { b = (b) ? false : true; }
void SimulationInputStatusSet::setConditionTrue(bool& b) { b = true; }
void SimulationInputStatusSet::setConditionFalse(bool& b) { b = false; }
float SimulationInputStatusSet::getStatusAndReset(float& f, float resetTo) { float ret = f; f = resetTo; return ret; }

