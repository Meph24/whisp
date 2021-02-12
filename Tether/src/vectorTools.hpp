/*
 * vectorTools.hpp
 *
 *  Created on:	02.02.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_VECTORTOOLS_HPP_
#define SRC_VECTORTOOLS_HPP_

#include <vector>

template<typename T>
void removeElement(std::vector<T> & v,T elem)
{
	size_t size=v.size();
	for(size_t i=0;i<size;i++)
	{
		if (v[i]==elem)
		{
			v.erase(v.begin()+i);
			break;
		}
	}
}
template<typename T>
void removeElementUnordered(std::vector<T> & v,T elem)
{
	size_t size=v.size();
	for(size_t i=0;i<size;i++)
	{
		if (v[i]==elem)
		{
			v[i]=v[size-1];
			v.pop_back();
			break;
		}
	}
}
template<typename T>
void removeIndexUnordered(std::vector<T> & v,size_t index)
{
	size_t size=v.size();
	v[index]=v[size-1];
	v.pop_back();
}

#endif /* SRC_VECTORTOOLS_HPP_ */
