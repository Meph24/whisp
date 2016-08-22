
#ifndef BUFFER_CPP
#define BUFFER_CPP

#include "Buffer.h"


template <typename T, int size >
Buffer<T, size>::Buffer()
{
	begin = &buffer[0];
	end = &buffer[size]; // one behind the end
	read = begin;
	dataend = begin;
}

template <typename T, int size >
bool Buffer<T, size>::nodata()
{
	return read == dataend;
}

template <typename T, int size >
bool Buffer<T, size>::isfull()
{
	return dataend == end;
}

template <typename T, int size >
void Buffer<T, size>::empty()
{
	dataend = begin;
	read = begin;
}

template <typename T, int size >
T Buffer<T, size>::readnext()
{
	if (nodata()) *(read--);

	T*save = read;
	read++;
	return *save;
}

template <typename T, int size >
void Buffer<T, size>::add(T value)
{
	if (isfull()) return;

	*dataend = value;
	dataend++;

}


#endif