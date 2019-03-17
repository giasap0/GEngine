#include "GVector3.h"

using namespace GMath;

GVector3 GMath::operator*(const Real& d, const GVector3& vect)		{	return vect*d;	}

GVector3::GVector3(const Real& norm, const GVector3& parallelTo)
{
	this->operator=(parallelTo.normalized() );
	for(int i=0; i<3; ++i)
		_v[i]=_v[i]*norm;
}

//they compares all elements
bool GVector3::operator ==(const GVector3& vect) const
{
	for(int i=0; i<3; ++i)
	{
		if( !areEqual( _v[i] , vect[i] ) )
			return false;
	}
	return true;
}
GVector3 GVector3::operator +(const GVector3& vect) const
{
	GVector3 r(*this);
	for(int i=0; i<3; ++i)
	{
		r[i]+=vect[i];
	}
	return r;
}
GVector3& GVector3::operator +=(const GVector3& vect)
{
	for(int i=0; i<3; ++i)
	{
		_v[i]+=vect[i];
	}
	return *this;
}
GVector3 GVector3::operator -(const GVector3& vect) const
{
	GVector3 r(*this);
	for(int i=0; i<3; ++i)
	{
		r[i]-=vect[i];
	}
	return r;
}
GVector3& GVector3::operator -=(const GVector3& vect)
{
	for(int i=0; i<3; ++i)
	{
		_v[i]-=vect[i];
	}
	return *this;
}
GVector3 GVector3::operator^(const GVector3& vect) const
{
	GVector3 r;
	 r[0]= (_v[1]*vect[2])-(_v[2]*vect[1]);
	 r[1]= (_v[2]*vect[0])-(_v[0]*vect[2]);
	 r[2]= (_v[0]*vect[1])-(_v[1]*vect[0]);
     return r;
}
GVector3& GVector3::operator^=(const GVector3& vect)
{
	Real temp[3];
	temp[0]= (_v[1]*vect[2])-(_v[2]*vect[1]);
	temp[1]= (_v[2]*vect[0])-(_v[0]*vect[2]);
	temp[2]= (_v[0]*vect[1])-(_v[1]*vect[0]);
	_v[0]=temp[0];
	_v[1]=temp[1];
	_v[2]=temp[2];
	return *this;
}
Radians GVector3::angleRadians(const GVector3& w) const
{
	Real scalar = this->operator *(w.normalized());
	scalar/=norm();
	//scalar == cos(alfa)
	return arcCos(scalar);
}
GVector3 GVector3::projectedTo(const GVector3& other) const
{
	GVector3 otherNormalized = other.normalized();
	Real newNorm = this->operator *(otherNormalized); 
	return otherNormalized*newNorm;
}
void GVector3::ortonormalBasis(GVector3* arrayOfVectors) const
{	
	arrayOfVectors[0]=normalized();	//==tau
	//find mu | mu*tau==0
	if(arrayOfVectors[0][2]!=.0f && areEqual(arrayOfVectors[0][1],.0f) )		//se è piano x-z ruoti attorno a y
	{
		arrayOfVectors[1][0]=arrayOfVectors[0][2];					//  0 0 1
        arrayOfVectors[1][1]=arrayOfVectors[0][1];					//  0 1 0
        arrayOfVectors[1][2]=-arrayOfVectors[0][0];					//  -1 0 0
	}
	else if(arrayOfVectors[0][2]!=.0f && areEqual(arrayOfVectors[0][0],.0f))	//se piano y-z ruoti attorno a x
	{
		arrayOfVectors[1][0]= arrayOfVectors[0][0];					//  1 0 0
		arrayOfVectors[1][1]=-arrayOfVectors[0][2];					//  0 0 -1
        arrayOfVectors[1][2]= arrayOfVectors[0][1];					//  0 1 0
	}
	else if( areEqual( arrayOfVectors[0][2], 0.0f ) )							//se piano x-y ruoti attorno a z
	{
		arrayOfVectors[1][0]=-arrayOfVectors[0][1];					//  0 -1 0
        arrayOfVectors[1][1]= arrayOfVectors[0][0];					//  1  0 0
        arrayOfVectors[1][2]= arrayOfVectors[0][2];					//  0  0 1
	}
	else															//piano qualsiasi
	{
		arrayOfVectors[1] = arrayOfVectors[0]^GVector3(0,1,0);
	}
	//n == tau^mu
	arrayOfVectors[2] = arrayOfVectors[0]^arrayOfVectors[1];
}

void GVector3::clamp(Real min, Real max)
{
	for(int i=0; i<3;i++)
	{
		if(_v[i]<min)
			_v[i]=min;
		if(_v[i]>max)
			_v[i]=max;
	}
}
void GVector3::clamp(const GVector3& min, const GVector3& max)
{
	for(int i=0; i<3;i++)
	{
		if(_v[i]<min[i])
			_v[i]=min[i];
		if(_v[i]>max[i])
			_v[i]=max[i];
	}
}
void GVector3::clamp(const GVector3& min, Real max)
{
	for(int i=0; i<3;i++)
	{
		if(_v[i]<min[i])
			_v[i]=min[i];
		if(_v[i]>max)
			_v[i]=max;
	}
}
void GVector3::clamp(Real min, const GVector3& max)
{
	for(int i=0; i<3;i++)
	{
		if(_v[i]<min)
			_v[i]=min;
		if(_v[i]>max[i])
			_v[i]=max[i];
	}
}


bool GVector3::isNull() const
{
	if( areEqual( _v[0], 0.0f ) && areEqual( _v[1], 0.0f ) && areEqual( _v[2], 0.0f ) )
		return true;
	return false;
}
bool GVector3::isParallelTo(const GVector3& vect) const
{
	Real k = normalized() * vect.normalized();
	if( areEqual(k,1.0f) )
		return true;
	return false;
}
bool GVector3::isNormalTo(const GVector3& vect) const
{
	Real k = this->operator*( vect );
	if( areEqual(k, 0.0f ) )
		return true;
	return false;
}
