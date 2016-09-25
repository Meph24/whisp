#ifndef LOOKUP_CPP
#define LOOKUP_CPP


#include "lookup.h"

template <typename T>
Lookup<T>::Lookup(unsigned int size) :
size(size),
validIndex(size, false)
{
	arr = new T[size];
}

template <typename T>
Lookup<T>::~Lookup()
{
	delete[] arr;
}

template <typename T>
lookup_index Lookup<T>::nextfreeindex()
{
	for (unsigned int i = 0; i < size; i++)
	{
		if (!validIndex.get(i)) return i;
	}
	return LOOKUP_NOVALIDINDEX;
}

template <typename T>
lookup_index Lookup<T>::insert(T value)
{
	//save insert , do not override existing data
	lookup_index ret;
	if (findkey(value, ret)) return LOOKUP_NOVALIDINDEX;

	ret = nextfreeindex();
	if (ret != LOOKUP_NOVALIDINDEX)
	{
		map.insert(std::pair<T, lookup_index>(value, ret));
		arr[ret] = value;
		validIndex.set(ret, true);
	}
	return ret;
}

template <typename T>
void Lookup<T>::erase(lookup_index index)
{
	if (validIndex.get(index))
	{
		validIndex.set(index, false);
		map.erase(map.find(arr[index]));
	}
}

template <typename T>
bool Lookup<T>::findkey(T value, lookup_index& ret)
{
	std::map<T, lookup_index>::iterator it = map.find(value);
	if (it == map.end()) return false;
	ret = it->second;
	return true;
}

template <typename T>
bool Lookup<T>::findvalue(lookup_index index, T& ret)
{
	if (validIndex.get(index))
	{
		ret = arr[index];
		return true;
	}
	return false;
}


/*
template <typename T>
lookup<T>::

*/

#endif