#pragma once

#include <iostream>

template <typename T, int size>
class Buffer
{
	T data [size];
	unsigned int borderOffset, writePtrOffset, readPtrOffset;

	inline T* getborder() const;

	inline T* getwrite() const;

	inline T* getread() const;
		
	inline unsigned int beginOffset() const;

	inline unsigned int endOffset() const;

	void fit();

	void repairOffsets();

	void writeat(unsigned int index, T value);

	T readat(unsigned int index);

public:
	Buffer();

	~Buffer();

	bool write(T* container, unsigned int amount);

	bool read(T* container, unsigned int amount);

	bool write(T value);

	bool read(T& value);

	unsigned int getfreespace() const;

	bool isfull() const;

	bool nodata() const;

	void empty();
	

	friend std::ostream& operator<< (std::ostream& os, const Buffer& b)
	{
		os << "____________________________________________" << std::endl;
		os << "Free space : " << b.getfreespace() << endl;
		os << "Full : " << b.isfull() << endl;
		os << "Nodata : " << b.nodata() << endl;
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
};



#include "Buffer.cpp"
