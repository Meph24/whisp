/*
 * EntityIdent.h
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ENTITYIDENT_H_
#define SRC_ENTITYIDENT_H_

class EntityIdent
{
public:
	unsigned int group;
	unsigned int insideID;

	bool isInGroup(unsigned int groupID);

	EntityIdent();
	~EntityIdent();
};

#endif /* SRC_ENTITYIDENT_H_ */
