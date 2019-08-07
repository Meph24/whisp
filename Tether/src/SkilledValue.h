/*
 * SkilledValue.h
 *
 *  Created on:	12.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_SKILLEDVALUE_H_
#define SRC_SKILLEDVALUE_H_

class SkilledValue
{
	float baseValue;
	float skillValue;
public:
	SkilledValue(float rookieValue,float masterValue);
	SkilledValue();
	~SkilledValue();
	bool isSkillInfluenced();
	float getValue(float skill);//0=rookie 1=master
};

#endif /* SRC_SKILLEDVALUE_H_ */
