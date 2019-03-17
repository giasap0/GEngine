#include "GVector2.h"

using namespace GMath;

GVector2 GMath::operator*(const Real& d, const GVector2& vect)		{	return vect*d;	}

GVector2::GVector2(const Real& norm, const GVector2& parallelTo)
{
	this->operator=(parallelTo.normalized() );
	for(int i=0; i<2; ++i)
		_v[i]=_v[i]*norm;
}

//they compares all elements
bool GVector2::operator ==(const GVector2& vect) const
{
	for(int i=0; i<2; ++i)
	{
		if( !areEqual( _v[i] , vect[i] ) )
			return false;
	}
	return true;
}
GVector2 GVector2::operator +(const GVector2& vect) const
{
	GVector2 r(*this);
	for(int i=0; i<2; ++i)
	{
		r[i]+=vect[i];
	}
	return r;
}
GVector2& GVector2::operator +=(const GVector2& vect)
{
	for(int i=0; i<2; ++i)
	{
		_v[i]+=vect[i];
	}
	return *this;
}
GVector2 GVector2::operator -(const GVector2& vect) const
{
	GVector2 r(*this);
	for(int i=0; i<2; ++i)
	{
		r[i]-=vect[i];
	}
	return r;
}
GVector2& GVector2::operator -=(const GVector2& vect)
{
	for(int i=0; i<2; ++i)
	{
		_v[i]-=vect[i];
	}
	return *this;
}

Radians GVector2::angleRadians(const GVector2& w) const
{
	Real scalar = this->operator *(w.normalized());
	scalar/=norm();
	//scalar == cos(alfa)
	return arcCos(scalar);
}
GVector2 GVector2::projectedTo(const GVector2& other) const
{
	GVector2 otherNormalized = other.normalized();
	Real newNorm = this->operator *(otherNormalized); 
	return otherNormalized*newNorm;
}
void GVector2::ortonormalBasis(GVector2* arrayOfVectors) const
{	
	arrayOfVectors[0]=normalized();	//==tau
	arrayOfVectors[1][0] = -arrayOfVectors[0][1];
	arrayOfVectors[1][1] = arrayOfVectors[0][0];
}

/*
Clamp() Returns x clamped to the range [a,b] as follows:

1) Returns a if x is less than a; else
2) Returns b if x is greater than b; else
3) Returns x otherwise.
For vectors, the returned vector contains the clamped result of each element of the vector x clamped using the respective element of vectors a and b.
*/

void GVector2::clamp(Real min, Real max)
{
	if(_v[0]<min)
		_v[0]=min;
	if(_v[0]>max)
		_v[0]=max;
	if(_v[1]<min)
		_v[1]=min;
	if(_v[1]>max)
		_v[1]=max;
}
void GVector2::clamp(const GVector2& min, const GVector2& max)
{
	if(_v[0]<min[0])
		_v[0]=min[0];
	if(_v[0]>max[0])
		_v[0]=max[0];
	if(_v[1]<min[1])
		_v[1]=min[1];
	if(_v[1]>max[1])
		_v[1]=max[1];
}
void GVector2::clamp(const GVector2& min, Real max)
{
	if(_v[0]<min[0])
		_v[0]=min[0];
	if(_v[0]>max)
		_v[0]=max;
	if(_v[1]<min[1])
		_v[1]=min[1];
	if(_v[1]>max)
		_v[1]=max;
}
void GVector2::clamp(Real min, const GVector2& max)
{
	if(_v[0]<min)
		_v[0]=min;
	if(_v[0]>max[0])
		_v[0]=max[0];
	if(_v[1]<min)
		_v[1]=min;
	if(_v[1]>max[1])
		_v[1]=max[1];
}

bool GVector2::isNull() const
{
	if( areEqual( _v[0], 0.0f ) && areEqual( _v[1], 0.0f ) )
		return true;
	return false;
}
bool GVector2::isParallelTo(const GVector2& vect) const
{
	Real k = normalized() * vect.normalized();
	if( areEqual(k,1.0f) )
		return true;
	return false;
}
bool GVector2::isNormalTo(const GVector2& vect) const
{
	Real k = this->operator*( vect );
	if( areEqual(k, 0.0f ) )
		return true;
	return false;
}
