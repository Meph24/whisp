
#ifndef BUFFER_CPP
#define BUFFER_CPP

#include "Buffer.h"


template <typename T, int size >
Buffer<T, size>::Buffer()
	:
	borderOffset(0),
	writePtrOffset(0)
{
}

template <typename T, int size >
Buffer<T, size>::~Buffer()
{
}

template <typename T, int size >
T* Buffer<T, size>::getborder() const
{
	return data + (borderOffset% size);
}
template <typename T, int size >
T* Buffer<T, size>::getwrite() const
{
	return data + (writePtrOffset% size);
}
template <typename T, int size >
T* Buffer<T, size>:: getread() const
{
	return data + (beginOffset()% size);
}
template <typename T, int size >
unsigned int Buffer<T, size>:: beginOffset() const
{
	return borderOffset;
}
template <typename T, int size >
unsigned int Buffer<T, size>::endOffset() const
{
	return borderOffset + size;
}

template <typename T, int size >
unsigned int Buffer<T, size>::getfreespace() const
{
	return endOffset() - writePtrOffset;
}



template <typename T, int size >
void Buffer<T, size>::repairOffsets()
{
	if (borderOffset > size)
	{
		// get offsets and the "round count" the offset actually has
		unsigned int borderdiv = borderOffset / size;
		unsigned int borderoff = borderOffset % size;
		
		unsigned int writediv = writePtrOffset / size;
		unsigned int writeoff = writePtrOffset % size;

		//set the roundcount as low as possible
		writediv -= borderdiv;

		// apply new values
		borderOffset = borderoff;
		writePtrOffset = writediv * size + writeoff;
	}
}



template <typename T, int size>
bool Buffer<T, size>::isfull() const
{
	return writePtrOffset >= endOffset();
}

template <typename T, int size>
bool Buffer<T, size>::nodata() const
{
	return beginOffset() >= writePtrOffset;
}

template <typename T, int size>
void Buffer<T, size>::empty()
{
	borderOffset = writePtrOffset;
}

template <typename T, int size>
void Buffer<T, size>::writeat(unsigned int index, T value)
{
	data[index%size] = value;
}

template <typename T, int size>
T Buffer<T, size>::readat(unsigned int index)
{
	return data[index%size];
}

//TODO Possible optimization by using memcpy instead of for loops in read and write

template <typename T, int size>
bool Buffer<T, size>::write(T* container, unsigned int amount)
{
	if ((writePtrOffset + amount) > endOffset()) return false;

	for (size_t i = 0; i < amount; i++)
	{
		writeat(writePtrOffset + i, container[i]);
	}
	writePtrOffset += amount;
	return true;
}


template <typename T, int size>
bool Buffer<T, size>::write(T value)
{
	return write(&value, 1);
}

template <typename T, int size>
bool Buffer<T, size>::read(T* container, unsigned int amount)
{
	if ((beginOffset()+amount) > writePtrOffset) return false;
	
	for (size_t i = 0; i < amount; i++)
	{
		container[i] = readat(readPtrOffset + i);
	}
	borderOffset += amount;
	repairOffsets();
	return true;
}

template <typename T, int size>
bool Buffer<T, size>::read(T& value)
{
	T container[1];
	bool save = read(container, 1);
	value = *container;
	return save;
}

#endif
