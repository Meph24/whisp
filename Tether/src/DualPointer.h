/*
 * DualPointer.h
 *
 *  Created on:	May 16, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_DUALPOINTER_H_
#define SRC_DUALPOINTER_H_
class Entity;

//Dual Pointer to hold a Pointer to an Entity and some other interface,
//which is both the same object, which allows a class to inherit many
//different interfaces (mostly used for physics interactions)
template<typename PhysicsIF>
struct DualPointer
{
	Entity * e;
	PhysicsIF * pIF;
	DualPointer(Entity * en,PhysicsIF * phIF);
	~DualPointer();
};

template<typename PhysicsIF>
inline DualPointer<PhysicsIF>::DualPointer(Entity* en, PhysicsIF* phIF):
e(en),pIF(phIF)
{}

template<typename PhysicsIF>
inline DualPointer<PhysicsIF>::~DualPointer()
{}

template<typename PhysicsIF>
DualPointer<PhysicsIF> makeDualPointer(Entity* e, PhysicsIF* pIF)
{
	return DualPointer<PhysicsIF>(e, pIF);
}

#endif /* SRC_DUALPOINTER_H_ */
