/*
 * ItemIdent.h
 *
 *  Created on:	Jan 9, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMIDENT_H_
#define SRC_ITEMIDENT_H_

#define ITEM_GROUP_UNIQUE_TICK 0
#define ITEM_GROUP_UNIQUE_NOTICK 1
#define ITEM_GROUP_FIRST_FREE 2


class ItemIdent
{
public:
	unsigned int group;
	unsigned int insideID;

	bool isUnique();
	bool needsTick();//always false if item is not unique
	bool isInGroup(unsigned int groupID);
	//TODO equal operator
	ItemIdent();
	~ItemIdent();
};

#endif /* SRC_ITEMIDENT_H_ */
