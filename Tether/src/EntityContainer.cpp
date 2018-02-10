/*
 * EntityContainer.cpp
 *
 *  Created on:	13.03.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "EntityContainer.h"
//TODO
/*
EntityContainer::EntityContainer():
nextUID(0)
{
//TODO
}

EntityContainer::~EntityContainer()
{
//TODO
}

entityID EntityContainer::insertEntity(Entity * blank)
{
	return {0,0};//TODO e2[mynewfuckingID]=blank;
}

void EntityContainer::advanceTick()
{
	Entity ** old1=e1;
	Entity ** old2=e2;
	Entity ** old3=e3;
	e2=old3;
	e1=old2;
	e3=old1;
}

entityPtr EntityContainer::getEntity(entityID id)
{
	entityPtr ret={0,0,0};
	Entity * p=e1[id.aID];
	if(p) if((p->UID)==id.uID) ret.past=p;
	p=e2[id.aID];
	if(p) if((p->UID)==id.uID) ret.now=p;
	p=e3[id.aID];
	if(p) if((p->UID)==id.uID) ret.future=p;
	return ret;
}


template <typename T>
class EntPtr
{
    EntHandler& enthandler;
public:
    EntPtr(EntHandler& enthandler);
    T& operator->();
    typedef struct
    {
        ID aID;
        ID uID;
    } entityID;
}

template<typename T>
EntPtr<T>::EntPtr(EntHandler& enthandler):
enthandler(enthandler)
{}

template<typename T>
T& EntPtr<T>::operator->()
{
    return *(enthandler.get());
}*/
