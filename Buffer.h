#pragma once

#include <iostream>

/*
//	Template Buffer class:
//		<T, size> _:
//		T :		Type of items to buffer
//		size:	Maximal amount of items to buffer
//		
//	Buffer is not dynamic : 
//			add() can be called exactly >size< times before the buffer must be emptied again (empty())
//	The term >readable data< is considered data which stand before the dataend pointer in the buffer
//			It is data which is not be invalidated by emptying the buffer or data in the memory at the time of allocation
//	All Methods return ! Meaning the state of the buffer is noted in the methods but you must watch them yourself!
//	examples:
//
//			Buffer < int, 5 > b;
//			
//			while (!b.isfull())
//			{
//			b.add(1024);
//			}
//			
//			while (!b.nodata())
//			{
//			cout << "read: " << b.readnext();
//			}
*/
template <typename T, int size>
class Buffer
{
	/*
	// allocated space
	*/
	T buffer[size];


	/*
	//	characteristic pointers on a buffer
	*/
	T* begin;
	T* end;
	T* dataend;
	T* read;

public:
	/*
	//	Buffer creation of template size with 0 elements already in the buffer
	*/
	Buffer();

	/*
	//	method to check if there is any unread data in the buffer
	*/
	bool nodata();

	/*
	//	method to check if there is no more space in the buffer to add new items
	*/
	bool isfull();

	/*
	//	method to empty the buffer (sets read and dataend pointers to begin)
	//	data is actually not deleted but the read() method will not read it
	//	new data is written on top of old
	*/
	void empty();

	/*
	//	returns the next unread item in the buffer
	// returns the last readable item in the buffer if there are no more items to read
	*/
	T readnext();

	/*
	//	adds a new item at the back of the buffer
	//	if there is no space available for the item the function will do nothing
	//	Param : T value : item which shall be placed at the back of the buffer
	*/
	void add(T value);


	/*
	//	stream operator overloading for printing contents
	//	will print all readable data in the buffer (everything before the dataend pointer)
	*/
	friend std::ostream& operator<< (std::ostream& os, const Buffer& buffer)
	{
		T* runner = buffer.begin;
		os << "|";
		while (runner != buffer.dataend)
		{
			os << " " << *runner << " |";
			runner++;
		}


		os << std::endl;
			return os;
	}
};

#include "Buffer.cpp"
