
#ifndef BUFFER_CPP
#define BUFFER_CPP

#include "Buffer.h"


template <typename T, int size >
Buffer<T, size>::Buffer()
	:
	borderOffset(0),
	readPtrOffset(0),
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
	return data + (readPtrOffset% size);
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
void Buffer<T, size>::fit()
{
	borderOffset = readPtrOffset;
	repairOffsets();
}

template <typename T, int size >
void Buffer<T, size>::repairOffsets()
{
	if (borderOffset > size)
	{
		// get offsets
		unsigned int borderoff = borderOffset % size;
		unsigned int writeoff = writePtrOffset % size;
		unsigned int readoff = readPtrOffset % size;

		//get the "round count" the offset actually has
		unsigned int borderdiv = borderOffset / size;
		unsigned int writediv = writePtrOffset / size;
		unsigned int readdiv = readPtrOffset / size;

		//set the roundcount as low as possible
		writediv -= borderdiv;
		readdiv -= borderdiv;

		// apply new values
		borderOffset = borderoff;
		writePtrOffset = writediv * size + writeoff;
		readPtrOffset = readdiv * size + readoff;
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
	return readPtrOffset >= writePtrOffset;
}

template <typename T, int size>
void Buffer<T, size>::empty()
{
	borderOffset = readPtrOffset = writePtrOffset;
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
	if ((readPtrOffset+amount) > writePtrOffset) return false;
	
	for (size_t i = 0; i < amount; i++)
	{
		container[i] = readat(readPtrOffset + i);
	}
	readPtrOffset += amount;
	fit();
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