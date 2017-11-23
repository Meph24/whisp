/*
 * FileAccess.cpp
 *
 *  Created on:	Sep 12, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "FileAccess.h"
#include <fstream>

FileAccess::FileAccess()
{
	// TODO Auto-generated constructor stub
}

void FileAccess::invertEndian2b(void* memory, len length)
{
	short int * arr=(short int *) memory;
	for(len i=0;i<length;i++)
	{
		short int a=arr[i];
		char * locPtr=(char*)&a;
		char temp=locPtr[0];
		locPtr[0]=locPtr[1];
		locPtr[1]=temp;
		arr[i]=a;
	}
}

void FileAccess::invertEndian4b(void* memory, len length)
{
	int * arr=(int *) memory;
	for(len i=0;i<length;i++)
	{
		int a=arr[i];
		char * locPtr=(char*)&a;
		char temp=locPtr[0];
		locPtr[0]=locPtr[3];
		locPtr[3]=temp;
		temp=locPtr[1];
		locPtr[1]=locPtr[2];
		locPtr[2]=temp;
		arr[i]=a;
	}
}

void FileAccess::invertEndian8b(void* memory, len length)
{
	len * arr=(len *) memory;
	for(len i=0;i<length;i++)
	{
		len a=arr[i];
		char * locPtr=(char*)&a;
		char temp=locPtr[0];
		locPtr[0]=locPtr[7];
		locPtr[7]=temp;
		temp=locPtr[1];
		locPtr[1]=locPtr[6];
		locPtr[6]=temp;
		temp=locPtr[2];
		locPtr[2]=locPtr[5];
		locPtr[5]=temp;
		temp=locPtr[3];
		locPtr[3]=locPtr[4];
		locPtr[4]=temp;
		arr[i]=a;
	}
}

void FileAccess::invertEndianNb(void* memory, len length, int N)
{
	len * arr=(len *) memory;
	for(len i=0;i<length;i++)
	{
		len a=arr[i];
		char * locPtr=(char*)&a;
		for(int j=0;j<N/2;j++)
		{
			char temp=locPtr[j];
			locPtr[j]=locPtr[N-1-j];
			locPtr[N-1-j]=temp;
		}
		arr[i]=a;
	}
}

fileContents FileAccess::readFile(const char * filename)
{
	std::ifstream * ka=new std::ifstream(filename,std::ifstream::ate|std::ifstream::binary);
	//TODO find better way
	std::ifstream::pos_type pos=ka->tellg();

	ka->close();
	ka=new std::ifstream(filename,std::ifstream::binary);

	char * ret=new char[(long long int)pos];
	ka->read(ret,(long long int)pos);
}

bool FileAccess::writeFile(const char * filename,len length, void* data)
{
	std::ofstream * ka=new std::ofstream(filename,std::ofstream::binary);
	ka->write((char *)data,(long long int)length);
}

FileAccess::~FileAccess()
{
	// TODO Auto-generated destructor stub
}

