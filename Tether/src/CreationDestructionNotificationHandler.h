/*
 * CreationDestructionNotificationHandler.h
 *
 *  Created on:	22.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_CREATIONDESTRUCTIONNOTIFICATIONHANDLER_H_
#define SRC_CREATIONDESTRUCTIONNOTIFICATIONHANDLER_H_

#include "ShortNames.h"
#include <vector>
#include "vectorTools.hpp"

template<typename T>
class CreationDestructionListener;


/*	Handles creation and destruction events, so multiple components can work on the same set of objects
 */
template<typename T>
class CreationDestructionNotificationHandler
{
public:
	std::vector<CreationDestructionListener<T> *> registered;

	CreationDestructionNotificationHandler();
	virtual ~CreationDestructionNotificationHandler();

	void registerCreationDestructionListener(CreationDestructionListener<T> * l);
	void removeCreationDestructionListener(CreationDestructionListener<T> * l);


	void notifyCreation(T * obj);
	void notifyDestruction(T * obj);


	//move and copy protection
	CreationDestructionNotificationHandler(const CreationDestructionNotificationHandler&) = delete;
	CreationDestructionNotificationHandler(CreationDestructionNotificationHandler&&) = delete;
	CreationDestructionNotificationHandler& operator=(const CreationDestructionNotificationHandler&) = delete;
	CreationDestructionNotificationHandler& operator=(CreationDestructionNotificationHandler&&) = delete;
};

template<typename T>
inline void CreationDestructionNotificationHandler<T>::removeCreationDestructionListener(CreationDestructionListener<T>* l)
{
	removeElementUnordered(registered,l);
	removeElementUnordered(l->registeredAt,this);
}

#include "CreationDestructionListener.h"
#include "myAssert.h"

template<typename T>
inline CreationDestructionNotificationHandler<T>::CreationDestructionNotificationHandler()
{
}

template<typename T>
inline CreationDestructionNotificationHandler<T>::~CreationDestructionNotificationHandler()
{
	size_t curSize=registered.size();
	while(curSize>0)
	{
		removeCreationDestructionListener(registered.front());
		size_t newSize=registered.size();
		assert(newSize<curSize);
		curSize=newSize;
	}
}

template<typename T>
inline void CreationDestructionNotificationHandler<T>::registerCreationDestructionListener(CreationDestructionListener<T>* l)
{
	registered.push_back(l);
	l->registeredAt.push_back(this);
}

template<typename T>
inline void CreationDestructionNotificationHandler<T>::notifyCreation(T* obj)
{
	for(CreationDestructionListener<T> * e: registered)
	{
		e->notifyCreation(obj);
	}
}

template<typename T>
inline void CreationDestructionNotificationHandler<T>::notifyDestruction(T* obj)
{
	for(CreationDestructionListener<T> * e: registered)
	{
		e->notifyDestruction(obj);
	}
}

#endif /* SRC_CREATIONDESTRUCTIONNOTIFICATIONHANDLER_H_ */
