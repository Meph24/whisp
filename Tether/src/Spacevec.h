/*
 * Spacevec.h
 *
 *  Created on:	Mar 14, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SPACEVEC_H_
#define SRC_SPACEVEC_H_
#include <cmath>
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

	intfloat<I,F> operator*(double scalar);
	void operator*=(double scalar);
	intfloat<I,F> operator/(double scalar);
	void operator/=(double scalar);
	//TODO make better
	intfloat<I,F> operator*(float scalar);
	void operator*=(float scalar);
	intfloat<I,F> operator/(float scalar);
	void operator/=(float scalar);


	bool operator<(intfloat<I,F> other);
	bool operator<=(intfloat<I,F> other);
	bool operator>(intfloat<I,F> other);
	bool operator>=(intfloat<I,F> other);
	bool operator!=(intfloat<I,F> other);
	bool operator==(intfloat<I,F> other);

	float fSquared(float chunksPerMeter);
	double dSquared(double chunksPerMeter);
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
inline intfloat<I, F> intfloat<I, F>::operator *(float scalar)
{//TODO make better
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
inline void intfloat<I, F>::operator *=(float scalar)
{
	*this= *this*scalar;
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator /(float scalar)
{
	return *this*(1/scalar);
}

template<typename I, typename F>
inline void intfloat<I, F>::operator /=(float scalar)
{
	*this*=(1/scalar);
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator *(double scalar)
{//TODO make better
	intfloat<I, double> ret1;
	ret1.floatpart=scalar*floatpart;
	ret1.intpart=(I)ret1.floatpart;
	ret1.floatpart-=ret1.intpart;
	intfloat<I, double> ret2;
	ret2.floatpart=scalar*intpart;
	ret2.intpart=(I)ret2.floatpart;
	ret2.floatpart-=ret2.intpart;
	ret1+=ret2;
	intfloat<I,F> ret;
	ret.floatpart=(F)ret1.floatpart;
	ret.intpart=ret1.intpart;
	return ret;
}

template<typename I, typename F>
inline void intfloat<I, F>::operator *=(double scalar)
{
	*this= *this*scalar;
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator /(double scalar)
{
	return *this*(1/scalar);
}

template<typename I, typename F>
inline void intfloat<I, F>::operator /=(double scalar)
{
	*this*=(1/scalar);
}
template<typename I, typename F>
inline bool intfloat<I, F>::operator ==(intfloat<I, F> other)
{
	if(intpart!=other.intpart) return false;
	return floatpart==other.floatpart;
}


template<typename I, typename F>
inline float intfloat<I, F>::fSquared(float chunksPerMeter)
{
	float ret=intpart;
	ret+=floatpart;
	ret*=chunksPerMeter;
	return ret*ret;
}

template<typename I, typename F>
inline double intfloat<I, F>::dSquared(double chunksPerMeter)
{
	double ret=intpart;
	ret+=floatpart;
	ret*=chunksPerMeter;
	return ret*ret;
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

	float fLengthSq(float chunksPerMeter);
	double dLengthSq(double chunksPerMeter);
	float fLength(float chunksPerMeter);
	double dLength(double chunksPerMeter);

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


template<typename I, typename F>
inline float vec3if<I, F>::fLengthSq(float chunksPerMeter)
{
	return x.fSquared(chunksPerMeter)+y.fSquared(chunksPerMeter)+z.fSquared(chunksPerMeter);
}

template<typename I, typename F>
inline double vec3if<I, F>::dLengthSq(double chunksPerMeter)
{
	return x.dSquared(chunksPerMeter)+y.dSquared(chunksPerMeter)+z.dSquared(chunksPerMeter);
}

template<typename I, typename F>
inline float vec3if<I, F>::fLength(float chunksPerMeter)
{
	return sqrt(fLengthSq(chunksPerMeter));
}

template<typename I, typename F>
inline double vec3if<I, F>::dLength(double chunksPerMeter)
{
	return sqrt(dLengthSq(chunksPerMeter));
}

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
