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

	void correct();

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

	float fSquared(float chunkSize);
	double dSquared(double chunkSize);

};

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator +(intfloat<I, F> other)
{
	intfloat<I,F> ret;
	ret.floatpart=floatpart+other.floatpart;
	ret.intpart=intpart+other.intpart;
	ret.correct();
	return ret;
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator -(intfloat<I, F> other)
{
	intfloat<I,F> ret;
	ret.floatpart=floatpart-other.floatpart;
	ret.intpart=intpart-other.intpart;
	ret.correct();
	return ret;
}

template<typename I, typename F>
inline void intfloat<I, F>::operator +=(intfloat<I, F> other)
{
	intpart+=other.intpart;
	floatpart+=other.floatpart;
	correct();
}

template<typename I, typename F>
inline void intfloat<I, F>::operator -=(intfloat<I, F> other)
{
	intpart-=other.intpart;
	floatpart-=other.floatpart;
	correct();
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
	ret.intpart=0;
	ret.floatpart=scalar*floatpart;
	ret.correct();
	intfloat<I, F> ret2;
	ret2.intpart=0;
	ret2.floatpart=scalar*intpart;
	ret2.correct();
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
	return *this*(1.0f/scalar);
}

template<typename I, typename F>
inline void intfloat<I, F>::operator /=(float scalar)
{
	*this*=(1.0f/scalar);
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator *(double scalar)
{//TODO make better
	intfloat<I, double> ret1;
	ret1.intpart=0;
	ret1.floatpart=scalar*floatpart;
	ret1.correct();
	intfloat<I, double> ret2;
	ret2.intpart=0;
	ret2.floatpart=scalar*intpart;
	ret2.correct();
	ret1+=ret2;
	intfloat<I,F> ret;
	ret.floatpart=(F)ret1.floatpart;
	ret.intpart=ret1.intpart;
	return ret;
}

template<typename I,typename F>
std::ostream& operator<<(std::ostream &out, const struct intfloat<I, F> v);

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
inline float intfloat<I, F>::fSquared(float chunkSize)
{
	float ret=intpart;
	ret+=floatpart;
	ret/=chunkSize;
	return ret*ret;
}

template<typename I, typename F>
inline void intfloat<I, F>::correct()
{
	intfloat<I, F> debugBefore=*this;
	I change=(I)floatpart;
	if(floatpart<0) change--;
	intpart+=change;
	floatpart-=change;
	intfloat<I, F> debugAfter=*this;
	if(debugBefore.floatpart<-1000 || debugBefore.floatpart>1000 || debugBefore.intpart<-1000 ||debugAfter.intpart<-1000)
		std::cout<<"before: "<<debugBefore<<" after:  "<<debugAfter<<std::endl;
}

template<typename I, typename F>
inline double intfloat<I, F>::dSquared(double chunkSize)
{
	double ret=intpart;
	ret+=floatpart;
	ret/=chunkSize;
	return ret*ret;
}
template<typename I, typename F>
inline std::ostream& operator <<(std::ostream& out, const struct intfloat<I, F> v)
{
	out<<"[ ch: "<<v.intpart<<", in: "<<v.floatpart<<" ]";
	return out;
}

template<typename I,typename F>
struct vec3if
{
	intfloat<I, F> x;
	intfloat<I, F> y;
	intfloat<I, F> z;


	vec3if<I,F> operator+(vec3if<I,F> other);
	vec3if<I,F> operator-(vec3if<I,F> other);
	void operator+=(vec3if<I,F> other);
	void operator-=(vec3if<I,F> other);

	vec3if<I,F> operator*(double scalar);
	void operator*=(double scalar);
	vec3if<I,F> operator/(double scalar);
	void operator/=(double scalar);
	//TODO make better
	vec3if<I,F> operator*(float scalar);
	void operator*=(float scalar);
	vec3if<I,F> operator/(float scalar);
	void operator/=(float scalar);


	float fLengthSq(float chunkSize);
	double dLengthSq(double chunkSize);
	float fLength(float chunkSize);
	double dLength(double chunkSize);

};

template<typename I,typename F>
std::ostream& operator<<(std::ostream &out, const struct vec3if<I, F> v);


template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator +(vec3if<I, F> other)
{
	vec3if<I, F> ret;
	ret.x=x+other.x;
	ret.y=y+other.y;
	ret.z=z+other.z;
	return ret;
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator -(vec3if<I, F> other)
{
	vec3if<I, F> ret;
	ret.x=x-other.x;
	ret.y=y-other.y;
	ret.z=z-other.z;
	return ret;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator +=(vec3if<I, F> other)
{
	x+=other.x;
	y+=other.y;
	z+=other.z;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator -=(vec3if<I, F> other)
{
	x-=other.x;
	y-=other.y;
	z-=other.z;
}


template<typename I, typename F>
inline float vec3if<I, F>::fLengthSq(float chunkSize)
{
	return x.fSquared(chunkSize)+y.fSquared(chunkSize)+z.fSquared(chunkSize);
}

template<typename I, typename F>
inline double vec3if<I, F>::dLengthSq(double chunkSize)
{
	return x.dSquared(chunkSize)+y.dSquared(chunkSize)+z.dSquared(chunkSize);
}

template<typename I, typename F>
inline float vec3if<I, F>::fLength(float chunkSize)
{
	return sqrt(fLengthSq(chunkSize));
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator *(double scalar)
{
	vec3if<I, F> ret;
	ret.x=x*scalar;
	ret.y=y*scalar;
	ret.z=z*scalar;
	return ret;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator *=(double scalar)
{
	x*=scalar;
	y*=scalar;
	z*=scalar;
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator /(double scalar)
{
	double inv=1.0;
	return (*this)*scalar;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator /=(double scalar)
{
	double inv=1.0;
	(*this)*=inv;
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator *(float scalar)
{
	vec3if<I, F> ret;
	ret.x=x*scalar;
	ret.y=y*scalar;
	ret.z=z*scalar;
	return ret;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator *=(float scalar)
{
	x*=scalar;
	y*=scalar;
	z*=scalar;
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator /(float scalar)
{
	float inv=1.0f;
	return (*this)*scalar;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator /=(float scalar)
{
	float inv=1.0f;
	(*this)*=inv;
}

template<typename I, typename F>
inline double vec3if<I, F>::dLength(double chunkSize)
{
	return sqrt(dLengthSq(chunkSize));
}

template<typename I, typename F>
inline std::ostream& operator <<(std::ostream& out, const struct vec3if<I, F> v)
{
	out<<"\nx: "<<v.x<<"\ny: "<<v.y<<"\nz: "<<v.z<<"\n";
	return out;
}

typedef int chunkNum;
typedef intfloat<chunkNum,flt> spacelen;
typedef vec3if<chunkNum, flt> spacevec;


#endif /* SRC_SPACEVEC_H_ */
