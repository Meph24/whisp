/*
 * SkilledValue.cpp
 *
 *  Created on:	12.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "SkilledValue.h"


SkilledValue::SkilledValue(float rookieValue, float masterValue):
baseValue(rookieValue),skillValue(masterValue-rookieValue)
{}

SkilledValue::SkilledValue()
{}

SkilledValue::~SkilledValue()
{}

bool SkilledValue::isSkillInfluenced()
{
	return skillValue!=0;
}

float SkilledValue::getValue(float skill)
{
	return skill*skillValue+baseValue;
}
