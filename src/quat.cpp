#include "quat.h"

#include "myAssert.h"

quat::quat()
{

}

quat::quat(flt R,flt I,flt J,flt K):
r(R),i(I),j(J),k(K)
{

}

quat::quat(vec3 normVec,float angle)
{
	float half=angle/2;
	r=cos(half);
	i=sin(half)*normVec.x;
	j=sin(half)*normVec.y;
	k=sin(half)*normVec.z;
	assert((r*r+i*i+j*j+k*k>0.999f)&&(r*r+i*i+j*j+k*k<1.001f));
}


quat::~quat()
{

}

quat quat::operator*(quat other)
{
	return {
		r*other.r-i*other.i-j*other.j-k*other.k,
		r*other.i+i*other.r+j*other.k-k*other.j,
		r*other.j-i*other.k+j*other.r+k*other.i,
		r*other.k+i*other.j-j*other.i+k*other.r
	};
}

quat quat::operator+(quat other)
{
	return {
		r+other.r,
		i+other.i,
		j+other.j,
		k+other.k
	};
}

void quat::invert()
{
	i=-i;
	j=-j;
	k=-k;
}

quat quat::inverse()
{
	return {r,-i,-j,-k};
}

vec3 quat::operator *(vec3 point)
{
	quat ret=this[0]*quat(0,point.x,point.y,point.z);
	ret=ret*(this->inverse());//inline optimizes r part away
	return {ret.i,ret.j,ret.k};
}

flt quat::absSq()
{
	return r*r+i*i+j*j+k*k;
}

flt quat::abs()
{
	return sqrt(absSq());
}
