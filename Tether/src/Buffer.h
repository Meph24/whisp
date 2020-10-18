#pragma once

#include <iostream>

/*
//	Template class for a buffer data structure
//	/template param T:		the Type of data the buffer stores
//	/template param size:	the size of the buffer , determines the maximal amount of data for type T that can be stored
//
//
*/

template <typename T, int size>
class Buffer
{
	// The array of data the buffer is holding
	T data[size];

	// Offsets for pointer calculation
	unsigned int borderOffset, writePtrOffset;

	// returns a pointer to the border in the buffer where the begin or and 
	inline T* getborder() const;

	// returns a pointer after content of the buffer to write at
	inline T* getwrite() const;

	// returns a pointer at the begin of the unread data in the buffer
	inline T* getread() const;

	// calculates the index the buffer save space begins
	inline unsigned int beginOffset() const;

	// calculates the index the buffer save space ends
	inline unsigned int endOffset() const;

	// fit() matches the position of the beginOffset to the readOffset and is so freeing save space in the buffer of data that has already been read
	void fit();

	// repairOffsets() is called frequently on read() calls to decrease offset values to a lower basis and prevents from bugs throug overflow 
	void repairOffsets();

	// write at a specific index in a unlimited index range which is mapped to the size of the buffer
	void writeat(unsigned int index, T value);

	// read at a specific index in a unlimited index range which is mapped to the size of the buffer
	T readat(unsigned int index);

public:
	Buffer();

	~Buffer();

	/*
	//	write a number of datatypes to the buffer
	//	moves the write pointer along the buffer
	//	returns true if successful
	//	returns false if unsuccessful for example if there is not enough space in the buffer
	//	/param container :	a pointer to a set of data of the type the buffer stores
	//	/param amount :		the amount of data which will be transferred from container to the buffer
	*/
	bool write(T* container, unsigned int amount);

	/*
	//	read a number of datatypes to the buffer
	//	moves the read pointer along the buffer
	//	returns true if successful
	//	returns false if unsuccessful for example if there is not enough data in the buffer to read
	//	/param container :	a pointer to a set of data of the type the buffer stores
	//	/param amount :		the amount of data which will be transferred from the buffer to the container
	*/
	bool read(T* container, unsigned int amount);

	/*
	//	a write method for a singe datapiece
	*/
	bool write(T value);

	/*
	//	a read method for a single datapiece
	*/
	bool read(T& value);

	/*
	//	returns free space in the buffer in the amount of T's (types of data the buffer stores) the buffer can additionaly hold
	*/
	unsigned int getfreespace() const;

	/*
	//	returns true if the buffer is full
	//	returns false if there is minimal one space left to write in
	*/
	bool isfull() const;

	/*
	//	returns true if the buffer has no more valid data to read
	//	returns false if there is minimal one data piece that can be read
	*/
	bool nodata() const;

	/*
	//	empties the buffer completely from data
	*/
	void clear();


	//_test_begin
	friend std::ostream& operator<< (std::ostream& os, const Buffer& b)
	{
		os << "____________________________________________" << std::endl;
		os << "Free space : " << b.getfreespace() << std::endl;
		os << "Full : " << b.isfull() << std::endl;
		os << "Nodata : " << b.nodata() << std::endl;
		os << "BorderOffset : " << b.borderOffset << " | begin / end : " << b.beginOffset() << "/" << b.endOffset() << std::endl;
		os << "WriteOffset / ReadOffset : " << b.writePtrOffset << "/" << b.readPtrOffset << std::endl;
		os << "Buffer:" << std::endl;
		for (size_t i = 0; i < size; i++)
		{
			os << b.data[i] << "\t";
		}
		os << std::endl;
		os << "____________________________________________" << std::endl;

		return os;
	}
	//_test_end
};



#include "Buffer.cpp"
