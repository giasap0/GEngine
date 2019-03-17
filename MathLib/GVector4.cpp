#include "GVector4.h"

using namespace GMath;

GVector4 GMath::operator*(const Real& d, const GVector4& vect)		{	return vect*d;	}

GVector4::GVector4(const Real& norm, const GVector4& parallelTo)
{
	this->operator=(parallelTo.normalized() );
	for(int i=0; i<4; ++i)
		_v[i]=_v[i]*norm;
}

//they compares all elements
bool GVector4::operator ==(const GVector4& vect) const
{
	for(int i=0; i<4; ++i)
	{
		if( !areEqual( _v[i] , vect[i] ) )
			return false;
	}
	return true;
}
GVector4 GVector4::operator +(const GVector4& vect) const
{
	GVector4 r(*this);
	for(int i=0; i<4; ++i)
	{
		r[i]+=vect[i];
	}
	return r;
}
GVector4& GVector4::operator +=(const GVector4& vect)
{
	for(int i=0; i<4; ++i)
	{
		_v[i]+=vect[i];
	}
	return *this;
}
GVector4 GVector4::operator -(const GVector4& vect) const
{
	GVector4 r(*this);
	for(int i=0; i<4; ++i)
	{
		r[i]-=vect[i];
	}
	return r;
}
GVector4& GVector4::operator -=(const GVector4& vect)
{
	for(int i=0; i<4; ++i)
	{
		_v[i]-=vect[i];
	}
	return *this;
}
Real GVector4::operator*(const GVector4& vect) const
{
	Real r=0.0f;
	for(int i=0; i<4; ++i)
	{
		r+= (_v[i] )* (vect[i]);
	}
	return r;
}
GVector4 GVector4::operator^(const GVector4& vect) const
{
	GVector4 r;
	 r[0]= (_v[1]*vect[2])-(_v[2]*vect[1]);
	 r[1]= (_v[2]*vect[0])-(_v[0]*vect[2]);
	 r[2]= (_v[0]*vect[1])-(_v[1]*vect[0]);
	 if( areEqual(_v[3], 0.0f ) && areEqual(vect[3], 0.0f ) )
		 r[3]=0;
	 else
		 r[3] = 1;
     return r;
}
GVector4& GVector4::operator^=(const GVector4& vect)
{
	Real temp[3];
	temp[0]= (_v[1]*vect[2])-(_v[2]*vect[1]);
	temp[1]= (_v[2]*vect[0])-(_v[0]*vect[2]);
	temp[2]= (_v[0]*vect[1])-(_v[1]*vect[0]);
	 if( areEqual(_v[3], 0.0f ) && areEqual(vect[3], 0.0f ) )
		 _v[3]=0;
	 else
		 _v[3] = 1;
	 _v[0]=temp[0];
	 _v[1]=temp[1];
	 _v[2]=temp[2];
	 return *this;
}
Radians GVector4::angleRadians(const GVector4& w) const
{
	Real scalar = this->operator *(w.normalized());
	scalar/=norm();
	//scalar == cos(alfa)
	return arcCos(scalar);
}
GVector4 GVector4::projectedTo(const GVector4& other) const
{
	GVector4 otherNormalized = other.normalized();
	Real newNorm = this->operator *(otherNormalized); 
	return otherNormalized*newNorm;
}
void GVector4::ortonormalBasis(GVector4* arrayOfVectors) const
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
		arrayOfVectors[1] = arrayOfVectors[0]^GVector4(0,1,0,0);
		arrayOfVectors[1] = arrayOfVectors[1].normalized();
	}
	//n == tau^mu
	arrayOfVectors[2] = arrayOfVectors[0]^arrayOfVectors[1];
}

void GVector4::clamp(Real min, Real max)
{
	for(int i=0; i<4;i++)
	{
		if(_v[i]<min)
			_v[i]=min;
		if(_v[i]>max)
			_v[i]=max;
	}
}
void GVector4::clamp(const GVector4& min, const GVector4& max)
{
	for(int i=0; i<4;i++)
	{
		if(_v[i]<min[i])
			_v[i]=min[i];
		if(_v[i]>max[i])
			_v[i]=max[i];
	}
}
void GVector4::clamp(const GVector4& min, Real max)
{
	for(int i=0; i<4;i++)
	{
		if(_v[i]<min[i])
			_v[i]=min[i];
		if(_v[i]>max)
			_v[i]=max;
	}
}
void GVector4::clamp(Real min, const GVector4& max)
{
	for(int i=0; i<4;i++)
	{
		if(_v[i]<min)
			_v[i]=min;
		if(_v[i]>max[i])
			_v[i]=max[i];
	}
}


bool GVector4::isNull() const
{
	if( areEqual( _v[0], 0.0f ) && areEqual( _v[1], 0.0f ) && areEqual( _v[2], 0.0f ) && areEqual( _v[3], 0.0f ) )
		return true;
	return false;
}
bool GVector4::isParallelTo(const GVector4& vect) const
{
	Real k = normalized() * vect.normalized();
	if( areEqual(k,1.0f) )
		return true;
	return false;
}
bool GVector4::isNormalTo(const GVector4& vect) const
{
	Real k = this->operator*( vect );
	if( areEqual(k, 0.0f ) )
		return true;
	return false;
}
