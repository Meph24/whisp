/*
 * EntityContainer.h
 *
 *  Created on:	13.03.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITYCONTAINER_H_
#define SRC_ENTITYCONTAINER_H_
//TODO
/*
#include "Entity.h"

typedef struct
{
	ID aID;
	ID uID;
} entityID;

typedef struct
{
	Entity * past;
	Entity * now;
	Entity * future;
} entityPtr;

class EntityContainer
{
	ID nextUID;


	//pointers can be 0 or valid, if(e2[i]==0) e1[i] and e3[i] cannot both be valid
	Entity ** e1;//past
	Entity ** e2;//now
	Entity ** e3;//future


	ID arraySize;
public:

	//blank entity does not have an ID yet
	entityID insertEntity(Entity * blank);

	//return of existed
	//bool removeEntity(entityID id);//TODO


	bool existsNow(entityID id);

	void advanceTick();

	entityPtr getEntity(entityID id);


	EntityContainer();
	~EntityContainer();
};
*/
#endif /* SRC_ENTITYCONTAINER_H_ */
