#pragma once

#ifndef GVECTOR2_H
#define GVECTOR2_H

#include "GMathFunctions.h"
#include <GString.h>

namespace GMath
{

//sizeof float[2]
class GVector2
{
	inline friend std::ostream& operator<<(std::ostream& os,const GVector2& c)
	{
		os <<"{" <<c._v[0] << ", "<< c._v[1] << ", "<< c._v[2] << " }";
		return os;
	}
	friend GVector2 operator *(const Real& d, const GVector2& vect);

	//linear interpolation
	//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
	inline friend GVector2 Lerp(const GVector2& A, const GVector2& B, Real beta)
	{
		 beta = ( beta > 1.0f) ? 1.0f : (( beta < 0.0f) ? 0.0f :  beta);  // Clamp value to 0 to 1
		return ( ((1.0f-beta)*A ) + (beta*B) );
	}

	inline friend GVector2 SmoothStep(const GVector2& A, const GVector2& B, Real t)
	{
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t*t*(3.f - (2.f*t) );
		return Lerp(A,B,t);
	}

	inline friend GVector2 CatmullRom(const GVector2& pos0, const GVector2& pos1, const GVector2& pos2, const GVector2& pos3, Real t)
	{
		float t2 = t*t;
		float t3 = t2*t;
		float P0 = -t3 + 2.0f * t2 - t;								float P1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
		float P2 = -3.0f * t3 + 4.0f * t2 + t;						float P3 = t3 - t2;
		GVector2 result;
		result[0] = (P0 * pos0.x() + P1 * pos1.x() + P2 * pos2.x() + P3 * pos3.x() ) * 0.5f;
		result[1] = (P0 * pos0.y() + P1 * pos1.y() + P2 * pos2.y() + P3 * pos3.y() ) * 0.5f;
		return result;
	}

public:
	GVector2(void)											{_v[0]=0; _v[1]=0; }
	GVector2(Real x, Real y)								{ _v[0]=x; _v[1]=y;}
	explicit GVector2(const Real v[2])						{ _v[0]=v[0];  _v[1]=v[1]; }
	GVector2(const Real& norm, const GVector2& parallelTo);
	GVector2(const GVector2& other)							{_v[0]=other._v[0]; _v[1]=other._v[1]; }
	~GVector2(void)											{}

	GVector2& operator=(const GVector2& other)				{_v[0]=other._v[0]; _v[1]=other._v[1]; 	return *this;}

	//they compares all elements
	bool operator ==(const GVector2& vect) const;         
	inline bool operator !=(const GVector2& vect) const		{return !this->operator==(vect);}
	//they compares norms
	inline bool operator  >(const GVector2& vect) const		{return normSquare()>vect.normSquare();}
    inline bool operator  <(const GVector2& vect) const		{return normSquare()<vect.normSquare();}
    inline bool operator >=(const GVector2& vect) const		{return normSquare()>=vect.normSquare();}
    inline bool operator <=(const GVector2& vect) const		{return normSquare()<=vect.normSquare();}

	GVector2 operator +(const GVector2& vect) const;
    GVector2& operator +=(const GVector2& vect);
    GVector2 operator -(const GVector2& vect) const;
	inline GVector2 operator -() const						{GVector2 w(*this); for(int i=0; i<2; ++i) w[i]*=-1; return w;}
	GVector2& operator -=(const GVector2& vect);
	inline Real operator *(const GVector2& vect) const		{return  (_v[0]*vect[0]) + (_v[1]*vect[1] );}
    inline GVector2 operator *(Real k) const				{GVector2 w(*this); w[0]*=k; w[1]*=k; return w;}
    inline GVector2& operator *=(Real k)					{_v[0]*=k; _v[1]*=k; return *this;}
	inline GVector2 operator /(Real k) const				{GVector2 w(*this); w[0]/=k; w[1]/=k; return w;}
    inline GVector2& operator /=(Real k)					{_v[0]/=k; _v[1]/=k; return *this;}

	//access
    Real& operator [](uint index)							{return _v[index];}
    const Real& operator [](uint index) const				{return _v[index];}
	const Real at(uint index) const							{return _v[index];}
	inline Real* data()										{return &_v[0];}
	inline const Real& x() const							{return _v[0];}
	inline const Real& y() const							{return _v[1];}
	inline Real& x()										{return _v[0];}
	inline Real& y()										{return _v[1];}
	inline void setValues(Real x, Real y)					{ _v[0]=x; _v[1]=y; }
	inline GString toString() const							{return GString("{ ")+GString::number(_v[0])+" , "+GString::number(_v[1])+" }";	}

	inline Real norm() const								{return GMath::sqrt( normSquare() );}
	inline Real normSquare() const							{return (_v[0]*_v[0])+(_v[1]*_v[1]);}
	inline GVector2 normalized() const
	{
		GVector2 w(*this);
		Real normS = normSquare();
		if (!isZero(normS))
			w /= GMath::sqrt(normS);
		return w;
	}
	inline Real distanceTo(const GVector2& w) const			{GVector2 d = this->operator-(w); return d.norm();}
	Radians angleRadians(const GVector2& w) const;
	GVector2 projectedTo(const GVector2& other) const;
	//array must be allocated with new GVector2[2]
	void ortonormalBasis(GVector2* arrayOfVectors) const;
	void clamp(Real min, Real max);
	void clamp(const GVector2& min, const GVector2& max);
	void clamp(const GVector2& min, Real max);
	void clamp(Real min, const GVector2& max);
	//true == |all elements|<=epsilon
    bool isNull() const;
    bool isParallelTo(const GVector2& vect) const;
    bool isNormalTo(const GVector2& vect) const;
private:
	Real _v[2];
};


}		//EO namespace GMath
#endif // GVECTOR2_H


