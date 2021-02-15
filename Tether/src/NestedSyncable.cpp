/*
 * NestedSyncable.cpp
 *
 *  Created on:	10.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "NestedSyncable.h"

void NestedSyncable::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	//none that have to be known externally (per definition)
}

void NestedSyncable::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
	//none that have to be known externally (per definition)
}

u32 NestedSyncable::getClassID()
{
	return CLASS_ID_INVALID;//is not created in SyncableFactory, therefore no classID
}
