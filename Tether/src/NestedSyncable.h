/*
 * NestedSyncable.h
 *
 *  Created on:	10.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_NESTEDSYNCABLE_H_
#define SRC_NESTEDSYNCABLE_H_

#include "Syncable.h"

/* Nested variant of Syncable.
 * Only works as part of another Syncable.
 * Does not register as separate Syncable, but has the same (de)serialization interface.
 */
class NestedSyncable: public Syncable
{
public:
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();
};

#endif /* SRC_NESTEDSYNCABLE_H_ */
