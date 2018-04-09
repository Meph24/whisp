/*
 * Spacevec.h
 *
 *  Created on:	Mar 14, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SPACEVEC_H_
#define SRC_SPACEVEC_H_
#include "MatrixLib.h"
//I must be a signed integer type
//F must be a signed floating point type
template<typename I,typename F>
struct intfloat
{
	I intpart;
	F floatpart;

	intfloat<I,F> operator+(intfloat<I,F> other);
	intfloat<I,F> operator-(intfloat<I,F> other);
	void operator+=(intfloat<I,F> other);
	void operator-=(intfloat<I,F> other);

	intfloat<I,F> operator*(F scalar);
	void operator*=(F scalar);
	intfloat<I,F> operator/(F scalar);
	void operator/=(F scalar);


	bool operator<(intfloat<I,F> other);
	bool operator<=(intfloat<I,F> other);
	bool operator>(intfloat<I,F> other);
	bool operator>=(intfloat<I,F> other);
	bool operator!=(intfloat<I,F> other);
	bool operator==(intfloat<I,F> other);


};

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator +(intfloat<I, F> other)
{
	intfloat<I,F> ret;
	ret.floatpart=floatpart+other.floatpart;
	ret.intpart=intpart+other.intpart;
	if (ret.floatpart>=1)
	{
		ret.intpart++;
		ret.floatpart-=1;
	}
	return ret;
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator -(intfloat<I, F> other)
{
	intfloat<I,F> ret;
	ret.floatpart=floatpart+other.floatpart;
	ret.intpart=intpart+other.intpart;
	if (ret.floatpart<0)
	{
		ret.intpart--;
		ret.floatpart+=1;
	}
	return ret;
}

template<typename I, typename F>
inline void intfloat<I, F>::operator +=(intfloat<I, F> other)
{
	intpart+=other.intpart;
	floatpart+=other.floatpart;
	if(floatpart>=1)
	{
		intpart++;
		floatpart-=1;
	}
}

template<typename I, typename F>
inline void intfloat<I, F>::operator -=(intfloat<I, F> other)
{
	intpart+=other.intpart;
	floatpart+=other.floatpart;
	if(floatpart<0)
	{
		intpart--;
		floatpart+=1;
	}
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator <(intfloat<I, F> other)
{
	if(intpart<other.intpart) return true;
	if(intpart==other.intpart)
		if(floatpart<other.floatpart) return true;
	return false;
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator <=(intfloat<I, F> other)
{
	if(intpart<other.intpart) return true;
	if(intpart==other.intpart)
		if(floatpart<=other.floatpart) return true;
	return false;
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator >(intfloat<I, F> other)
{
	if(intpart>other.intpart) return true;
	if(intpart==other.intpart)
		if(floatpart>other.floatpart) return true;
	return false;
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator >=(intfloat<I, F> other)
{
	if(intpart>other.intpart) return true;
	if(intpart==other.intpart)
		if(floatpart>=other.floatpart) return true;
	return false;
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator !=(intfloat<I, F> other)
{
	if(intpart!=other.intpart) return true;
	return floatpart!=other.floatpart;
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator *(F scalar)
{
	intfloat<I, F> ret;
	ret.floatpart=scalar*floatpart;
	ret.intpart=(I)ret.floatpart;
	ret.floatpart-=ret.intpart;
	intfloat<I, F> ret2;
	ret2.floatpart=scalar*intpart;
	ret2.intpart=(I)ret2.floatpart;
	ret2.floatpart-=ret2.intpart;
	return ret+ret2;
}

template<typename I, typename F>
inline void intfloat<I, F>::operator *=(F scalar)
{
	*this= *this*scalar;
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator /(F scalar)
{
	return *this*(1/scalar);
}

template<typename I, typename F>
inline void intfloat<I, F>::operator /=(F scalar)
{
	*this*=(1/scalar);
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator ==(intfloat<I, F> other)
{
	if(intpart!=other.intpart) return false;
	return floatpart==other.floatpart;
}


template<typename I,typename F>
struct vec3if
{
	intfloat<I, F> x;
	intfloat<I, F> y;
	intfloat<I, F> z;


	intfloat<I,F> operator+(intfloat<I,F> other);
	intfloat<I,F> operator-(intfloat<I,F> other);
	void operator+=(intfloat<I,F> other);
	void operator-=(intfloat<I,F> other);

};


template<typename I, typename F>
inline intfloat<I, F> vec3if<I, F>::operator +(intfloat<I, F> other)
{
	intfloat<I, F> ret;
	ret.x=x+other.x;
	ret.y=y+other.y;
	ret.z=z+other.z;
	return ret;
}

template<typename I, typename F>
inline intfloat<I, F> vec3if<I, F>::operator -(intfloat<I, F> other)
{
	intfloat<I, F> ret;
	ret.x=x-other.x;
	ret.y=y-other.y;
	ret.z=z-other.z;
	return ret;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator +=(intfloat<I, F> other)
{
	x+=other.x;
	y+=other.y;
	z+=other.z;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator -=(intfloat<I, F> other)
{
	x-=other.x;
	y-=other.y;
	z-=other.z;
}

typedef int chunkNum;
typedef intfloat<chunkNum,flt> spacelen;
typedef vec3if<chunkNum, flt> spacevec;


/*template<typename I,typename F,unsigned int dim>
struct spacevec
{
	intfloat<I,F> arr[dim];

};*/

/*

//class ChunkManager;
//#include "ChunkManager.h"

#include "MatrixLib.h"
typedef int chint;
typedef float chflt;
typedef long long chunkMultResult;

struct exflt
{
	chflt in;
	chint chunk;

	exflt operator+(exflt other);
	exflt operator-(exflt other);
	void operator+=(exflt other);
	void operator-=(exflt other);

	bool operator<(exflt other);
	bool operator<=(exflt other);
	bool operator>(exflt other);
	bool operator>=(exflt other);
	bool operator==(exflt other);
	bool operator!=(exflt other);

	exflt operator+(chflt other);
	exflt operator-(chflt other);
	void operator+=(chflt other);
	void operator-=(chflt other);

	bool operator<(chflt other);
	bool operator<=(chflt other);
	bool operator>(chflt other);
	bool operator>=(chflt other);

	operator exflt(chflt toCast);
	operator chflt(exflt toCast);

};

class Position
{
	exflt x;
	exflt y;
	exflt z;
	ChunkManager * cm;
public:
	Position();//TODO
	~Position();

	Position operator+(Position other);
	Position operator-(Position other);
	void operator+=(Position other);
	void operator-=(Position other);

	unsigned int operator<(vec3 other);
	unsigned int operator>(vec3 other);


	void normalize();
	exflt normGetLen();//normalize this vec3 and return length of the unnormalized version
	Position normalizedCopy();//returns a normalized copy of itself

	exflt length();
	exflt lengthSq();//TODO precision?//return squared length
};

//vec3 operator*(flt scalar,vec3 v);
//flt dotProduct(vec3 a, vec3 b);
//vec3 crossProduct(vec3 a, vec3 b);
std::ostream& operator<<(std::ostream &out, const Position v);
*/
#endif /* SRC_SPACEVEC_H_ */
