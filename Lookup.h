
#ifndef LOOKUP_H
#define LOOKUP_H




/*
//	lookup<T> is a datastructure specialized in lookups, meaning efficient 2 way search of pairs of data
//	one of the data in this case in a index
//	both index and value can be called in find as key
//	the indices can not be chosen and are selected automatically in a way so that the smalles free index will be the index for a new inserted value
*/


#include <map>
#include "boolarrayx32.h"
typedef int lookup_index;
#define LOOKUP_NOVALIDINDEX -1 


template <typename T>
class Lookup
{
	T* arr;
	std::map<T, lookup_index> map;

	boolarrayx32 validIndex;


	/*
	//	stores the amount of data pairs  the lookup can hoold
	*/
	unsigned int size;

public:
	Lookup(unsigned int size);
	~Lookup();

	lookup_index nextfreeindex();

	//TODO save insert which does NOT override values inside ?
	lookup_index insert(T value);
	void erase(lookup_index index);

	bool findkey(T value, lookup_index& ret);
	bool findvalue(lookup_index index, T& ret);
};

#include "lookup.cpp"

#endif

