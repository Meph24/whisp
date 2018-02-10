/*
 * FileAccess.h
 *
 *  Created on:	Sep 12, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_FILEACCESS_H_
#define SRC_FILEACCESS_H_

#define endianDetect 252117761
#define oppositeEndian 16975631
typedef unsigned long long len;


typedef struct
{
	void * deleteThis;
	int endianState;//0=matching endianess, no action required; 1=opposite endian, -1=error in detection mechanism
	len lengthInBytes;//length of usefulData
	void * usefulData;//actual data content
} fileContents;


//useful for files that easily fit into RAM
class FileAccess
{

public:

	void invertEndian2b(void * memory,len length);
	void invertEndian4b(void * memory,len length);
	void invertEndian8b(void * memory,len length);
	void invertEndianNb(void * memory,len length, int N);

	fileContents readFile(const char * filename);//TODO url
	bool writeFile(const char * filename,len length,void * data);//TODO url

	FileAccess();
	~FileAccess();
};

#endif /* SRC_FILEACCESS_H_ */
