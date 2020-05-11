/*
 * Spacevec.h
 *
 *  Created on:	Mar 14, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SPACEVEC_H_
#define SRC_SPACEVEC_H_

#include <iostream>
#include <string>
#include "MathStuff.h"
#include "glmutils.hpp"
#include <glm/glm.hpp>
using glm::vec3;

//I must be a signed integer type
//F must be a signed floating point type
template<typename I,typename F>
struct intfloat
{
	I intpart;
	F floatpart;

	void correct();
	void correctPos();

	intfloat<I,F> operator+(intfloat<I,F> other) const;
	intfloat<I,F> operator-(intfloat<I,F> other) const;
	void operator+=(intfloat<I,F> other);
	void operator-=(intfloat<I,F> other);

	intfloat<I,F> operator*(double scalar) const;
	void operator*=(double scalar);
	intfloat<I,F> operator/(double scalar) const;
	void operator/=(double scalar);
	//TODO make better
	intfloat<I,F> operator*(float scalar) const;
	void operator*=(float scalar);
	intfloat<I,F> operator/(float scalar) const;
	void operator/=(float scalar);


	bool operator<(intfloat<I,F> other) const;
	bool operator<=(intfloat<I,F> other) const;
	bool operator>(intfloat<I,F> other) const;
	bool operator>=(intfloat<I,F> other) const;
	bool operator!=(intfloat<I,F> other) const;
	bool operator==(intfloat<I,F> other) const;

	bool equalsZero() const;//tiny epsilon tolerance
};

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator +(intfloat<I, F> other) const
{
	intfloat<I,F> ret;
	ret.floatpart=floatpart+other.floatpart;
	ret.intpart=intpart+other.intpart;
	ret.correctPos();
	return ret;
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator -(intfloat<I, F> other) const
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
	correctPos();
}

template<typename I, typename F>
inline void intfloat<I, F>::operator -=(intfloat<I, F> other)
{
	intpart-=other.intpart;
	floatpart-=other.floatpart;
	correct();
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator <(intfloat<I, F> other) const
{
	return (intpart<other.intpart)||((intpart==other.intpart)&&(floatpart<other.floatpart));
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator <=(intfloat<I, F> other) const
{
	return (intpart<other.intpart)||((intpart==other.intpart)&&(floatpart<=other.floatpart));
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator >(intfloat<I, F> other) const
{
	return (intpart>other.intpart)||((intpart==other.intpart)&&(floatpart>other.floatpart));
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator >=(intfloat<I, F> other) const
{
	return (intpart>other.intpart)||((intpart==other.intpart)&&(floatpart>=other.floatpart));
}

template<typename I, typename F>
inline bool intfloat<I, F>::operator !=(intfloat<I, F> other) const
{
	return (intpart!=other.intpart)||(floatpart!=other.floatpart);
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator *(float scalar) const
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
inline intfloat<I, F> intfloat<I, F>::operator /(float scalar) const
{
	return *this*(1.0f/scalar);
}

template<typename I, typename F>
inline void intfloat<I, F>::operator /=(float scalar)
{
	*this*=(1.0f/scalar);
}

template<typename I, typename F>
inline intfloat<I, F> intfloat<I, F>::operator *(double scalar) const
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
inline intfloat<I, F> intfloat<I, F>::operator/(double scalar)const
{
	return *this*(1/scalar);
}

template<typename I, typename F>
inline void intfloat<I, F>::operator /=(double scalar)
{
	*this*=(1/scalar);
}
template<typename I, typename F>
inline bool intfloat<I, F>::operator ==(intfloat<I, F> other) const
{
	return (intpart==other.intpart)&&(floatpart==other.floatpart);
}

template<typename I, typename F>
inline void intfloat<I, F>::correct()
{
	I change=(I)floatpart;
	change-=(floatpart<0);
	intpart+=change;
	floatpart-=change;
}

template<typename I, typename F>
inline void intfloat<I, F>::correctPos()
{
	I change=(I)floatpart;
	intpart+=change;
	floatpart-=change;
}

template<typename I, typename F>
inline std::ostream& operator<<(std::ostream& out, const struct intfloat<I, F> v)
{
	out << "[ ch: " << v.intpart << ", in: " << v.floatpart <<" ]";
	return out;
}

template<typename I, typename F>
inline bool intfloat<I, F>::equalsZero() const
{
	if(intpart!=0) return false;
	if(floatpart>0.0000000001f) return false;
	if(floatpart<-0.0000000001f) return false;
	return true;
}

template<typename I,typename F>
struct vec3if
{
	intfloat<I, F> x;
	intfloat<I, F> y;
	intfloat<I, F> z;


	vec3if<I,F> operator+(vec3if<I,F> other) const;
	vec3if<I,F> operator-(vec3if<I,F> other) const;
	vec3if<I,F> operator-() const;
	void operator+=(vec3if<I,F> other);
	void operator-=(vec3if<I,F> other);

	vec3if<I,F> operator*(double scalar) const;
	void operator*=(double scalar);
	vec3if<I,F> operator/(double scalar) const;
	void operator/=(double scalar);
	//TODO make better
	vec3if<I,F> operator*(float scalar) const;
	void operator*=(float scalar);
	vec3if<I,F> operator/(float scalar) const;
	void operator/=(float scalar);

	//low precision
	vec3 operator/(vec3if<I,F> other) const;

	//returns bit vector of comparison lsb=x
	unsigned int operator<(vec3if<I,F> other) const;
	unsigned int operator<=(vec3if<I,F> other) const;
	unsigned int operator>(vec3if<I,F> other) const;
	unsigned int operator>=(vec3if<I,F> other) const;

	bool equalsZero() const;//tiny epsilon tolerance
	intfloat<I,F> lengthHP() const;//gives the length using high precision (currently: double)
	intfloat<I,F> lengthLP() const;//gives the length using low precision (currently: float)

	template<typename P>
	intfloat<I,F> lengthTP() const;//gives the length using the given precision, as long as sqrt() exists for that precision

	void set0();

	float dot(vec3 v) const;//use only if already relativized, otherwise precision problems

	vec3if<I, F> selectWhere(int boolvec);
};

template<typename I,typename F>
std::ostream& operator<<(std::ostream &out, const struct vec3if<I, F> v);


template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator +(vec3if<I, F> other) const
{
	vec3if<I, F> ret;
	ret.x=x+other.x;
	ret.y=y+other.y;
	ret.z=z+other.z;
	return ret;
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator -(vec3if<I, F> other) const
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
inline bool vec3if<I, F>::equalsZero() const
{
	return x.equalsZero()&&y.equalsZero()&&z.equalsZero();
}

template<typename I, typename F>
template<typename P>
inline intfloat<I,F> vec3if<I,F>::lengthTP() const
{
	intfloat<I,F> ret;
	P r=0,
	temp=(P)x.intpart+(P)x.floatpart;
	r+=temp*temp;
	temp=(P)y.intpart+(P)y.floatpart;
	r+=temp*temp;
	temp=(P)z.intpart+(P)z.floatpart;
	r+=temp*temp;
	r=sqrt(r);
	ret.intpart=(I)r;
	ret.floatpart=r-ret.intpart;
	return ret;
}

template<typename I, typename F>
inline intfloat<I,F> vec3if<I,F>::lengthHP() const
{
	return lengthTP<double>();
}

template<typename I, typename F>
inline intfloat<I,F> vec3if<I,F>::lengthLP() const
{
	return lengthTP<float>();
}
template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator *(double scalar) const
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
inline vec3if<I, F> vec3if<I, F>::operator /(double scalar) const
{
	double inv=1.0/scalar;
	return (*this)*scalar;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator /=(double scalar)
{
	double inv=1.0/scalar;
	(*this)*=inv;
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator *(float scalar) const
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
inline vec3if<I, F> vec3if<I, F>::operator /(float scalar) const
{
	float inv=1.0f/scalar;
	return (*this)*inv;
}

template<typename I, typename F>
inline void vec3if<I, F>::operator /=(float scalar)
{
	float inv=1.0f/scalar;
	(*this)*=inv;
}

template<typename I, typename F>
inline unsigned int vec3if<I, F>::operator <(vec3if<I, F> other) const
{
	return (x<other.x)*1+(y<other.y)*2+(z<other.z)*4;
}

template<typename I, typename F>
inline unsigned int vec3if<I, F>::operator <=(vec3if<I, F> other) const
{
	return (x<=other.x)*1+(y<=other.y)*2+(z<=other.z)*4;
}

template<typename I, typename F>
inline unsigned int vec3if<I, F>::operator >(vec3if<I, F> other) const
{
	return (x>other.x)*1+(y>other.y)*2+(z>other.z)*4;
}

template<typename I, typename F>
inline unsigned int vec3if<I, F>::operator >=(vec3if<I, F> other) const
{
	return (x>=other.x)*1+(y>=other.y)*2+(z>=other.z)*4;
}

template<typename I, typename F>
inline std::ostream& operator <<(std::ostream& out, const struct vec3if<I, F> v)
{
	out<<"\nx: "<<v.x<<"\ny: "<<v.y<<"\nz: "<<v.z<<"\n";
	return out;
}


template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::operator -() const
{
	vec3if<I, F> zero;
	zero.x.floatpart=0;
	zero.x.intpart=0;
	zero.y.floatpart=0;
	zero.y.intpart=0;
	zero.z.floatpart=0;
	zero.z.intpart=0;
	return zero-*this;
}

template<typename I, typename F>
inline void vec3if<I, F>::set0()
{
	x.floatpart=0;
	x.intpart=0;
	y.floatpart=0;
	y.intpart=0;
	z.floatpart=0;
	z.intpart=0;
}

template<typename I, typename F>
inline float vec3if<I, F>::dot(vec3 v) const
{
	float ret=v.x*x.floatpart;
	ret+=v.x*x.intpart;
	ret+=v.y*y.floatpart;
	ret+=v.y*y.intpart;
	ret+=v.z*z.floatpart;
	ret+=v.z*z.intpart;
	return ret;
}

template<typename I, typename F>
inline vec3 vec3if<I, F>::operator /(vec3if<I, F> other) const
{
	vec3 ret;
	ret.x=(x.intpart+x.floatpart)/(other.x.intpart+other.x.floatpart);
	ret.y=(y.intpart+y.floatpart)/(other.y.intpart+other.y.floatpart);
	ret.z=(z.intpart+z.floatpart)/(other.z.intpart+other.z.floatpart);
	return ret;
}

template<typename I, typename F>
inline vec3if<I, F> vec3if<I, F>::selectWhere(int boolvec)
{
	vec3if<I, F> ret;
	ret.set0();
	if(boolvec&1) ret.x=x;
	if(boolvec&2) ret.y=y;
	if(boolvec&4) ret.z=z;
	return ret;
}

template<typename I, typename F>
struct chunkCoordinate
{
	I x;
	I y;
	I z;
	chunkCoordinate(const struct vec3if<I, F>& from);
	chunkCoordinate(I X,I Y,I Z);

	bool operator==(chunkCoordinate<I, F> other) const;
	bool operator!=(chunkCoordinate<I, F> other) const;
};


template<typename I, typename F>
inline chunkCoordinate<I, F>::chunkCoordinate(const struct vec3if<I, F>& from)
{
	x=from.x.intpart;
	y=from.y.intpart;
	z=from.z.intpart;
}

template<typename I, typename F>
inline bool chunkCoordinate<I, F>::operator ==(chunkCoordinate<I, F> other) const
{
	return (x==other.x)&&(y==other.y)&&(z==other.z);
}

template<typename I, typename F>
inline chunkCoordinate<I, F>::chunkCoordinate(I X, I Y, I Z):
x(X),y(Y),z(Z)
{
}

template<typename I, typename F>
inline bool chunkCoordinate<I, F>::operator !=(chunkCoordinate<I, F> other) const
{
	return !((*this)==other);
}


// 32 bit integer leads to ~7 light-seconds world radius (@gridSize=1m)
// 64 bit integer leads to ~970 light-years world radius (@gridSize=1m)
typedef int gridInt;
typedef intfloat<gridInt,float> spacelen;
typedef vec3if<gridInt, float> spacevec;
typedef chunkCoordinate<gridInt, float> chunkCoo;

#endif /* SRC_SPACEVEC_H_ */
