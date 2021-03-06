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
#define ITEM_GROUP_USELESS 3


class ItemIdent
{
public:
	unsigned int group;
	unsigned int insideID;//both identical mean item is identical (and can be stacked if stackable)

	bool isUnique();
	bool needsTick();//always false if item is not unique
	bool isInGroup(unsigned int groupID);
	bool operator==(ItemIdent other);

	ItemIdent();
	~ItemIdent();
};

#endif /* SRC_ITEMIDENT_H_ */
