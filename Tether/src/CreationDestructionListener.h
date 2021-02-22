/*
 * CreationDestructionListener.h
 *
 *  Created on:	22.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_CREATIONDESTRUCTIONLISTENER_H_
#define SRC_CREATIONDESTRUCTIONLISTENER_H_

#include "ShortNames.h"
#include <vector>

template<typename T>
class CreationDestructionNotificationHandler;


/*	Listens for creation and destruction events.
 *	Features protections from forgetting to unregister.
 */
template<typename T>
class CreationDestructionListener
{
public:
	std::vector<CreationDestructionNotificationHandler<T> *> registeredAt;

	CreationDestructionListener();
	virtual ~CreationDestructionListener();

	virtual void notifyCreation(T * obj)=0;
	virtual void notifyDestruction(T * obj)=0;


	//move and copy protection
	CreationDestructionListener(const CreationDestructionListener&) = delete;
	CreationDestructionListener(CreationDestructionListener&&) = delete;
	CreationDestructionListener& operator=(const CreationDestructionListener&) = delete;
	CreationDestructionListener& operator=(CreationDestructionListener&&) = delete;
};

#include "CreationDestructionNotificationHandler.h"

template<typename T>
inline CreationDestructionListener<T>::CreationDestructionListener()
{
}

template<typename T>
inline CreationDestructionListener<T>::~CreationDestructionListener()
{
	for(auto reg: registeredAt)
	{
		reg->removeCreationDestructionListener(this);
	}
}

#endif /* SRC_CREATIONDESTRUCTIONLISTENER_H_ */
