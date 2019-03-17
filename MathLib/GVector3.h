#pragma once
#ifndef GVECTOR3_H
#define GVECTOR3_H

#include "GMathFunctions.h"
#include <GString.h>

namespace GMath
{
//sizeof float[3]
class GVector3
{
	//linear interpolation
	//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
	inline friend GVector3 Lerp(const GVector3& A, const GVector3& B, Real beta)
	{
		beta = ( beta > 1.0f) ? 1.0f : (( beta < 0.0f) ? 0.0f :  beta);  // Clamp value to 0 to 1
		return ( ((1.0f-beta)*A ) + (beta*B) );
	}
	inline friend GVector3 CubicInterpolation(const GVector3& A, const GVector3& B, Real t)
	{
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t*t*(3.f - (2.f*t) );
		return Lerp(A,B,t);
	}
	//curve beteween pos1 and pos2. In pos1 is parallel to pos0-pos1 and in pos2 is parallel to po2-pos3
	inline friend GVector3 CatmullRom(const GVector3& pos0, const GVector3& pos1, const GVector3& pos2, const GVector3& pos3, Real t)
	{
		float t2 = t*t;
		float t3 = t2*t;
		float P0 = -t3 + 2.0f * t2 - t;								float P1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
		float P2 = -3.0f * t3 + 4.0f * t2 + t;						float P3 = t3 - t2;
		GVector3 result;
		result[0] = (P0 * pos0.x() + P1 * pos1.x() + P2 * pos2.x() + P3 * pos3.x() ) * 0.5f;
		result[1] = (P0 * pos0.y() + P1 * pos1.y() + P2 * pos2.y() + P3 * pos3.y() ) * 0.5f;
		result[2] = (P0 * pos0.z() + P1 * pos1.z() + P2 * pos2.z() + P3 * pos3.z() ) * 0.5f;
		return result;
	}

	inline friend std::ostream& operator<<(std::ostream& os,const GVector3& c)
	{
		os <<"{" <<c._v[0] << ", "<< c._v[1] << ", "<< c._v[2] << " }";
		return os;
	}
	friend GVector3 operator *(const Real& d, const GVector3& vect);

public:
	//Vector with all elements==0
	GVector3(void)											{_v[0]=0; _v[1]=0; _v[2]=0;}
	GVector3(Real x, Real y, Real z)						{ _v[0]=x; _v[1]=y; _v[2]=z;}
	explicit GVector3(const Real v[3])						{ _v[0]=v[0];  _v[1]=v[1];  _v[2]=v[2]; }
	GVector3(const Real& norm, const GVector3& parallelTo);
	GVector3(const GVector3& other)							{_v[0]=other._v[0]; _v[1]=other._v[1]; 	_v[2]=other._v[2]; }
	~GVector3(void)											{}

	GVector3& operator=(const GVector3& other)				{_v[0]=other._v[0]; _v[1]=other._v[1]; 	_v[2]=other._v[2]; return *this;}

	//they compares all elements
	bool operator ==(const GVector3& vect) const;         
	inline bool operator !=(const GVector3& vect) const		{return !this->operator==(vect);}
	//they compares norms
	inline bool operator  >(const GVector3& vect) const		{return normSquare()>vect.normSquare();}
    inline bool operator  <(const GVector3& vect) const		{return normSquare()<vect.normSquare();}
    inline bool operator >=(const GVector3& vect) const		{return normSquare()>=vect.normSquare();}
    inline bool operator <=(const GVector3& vect) const		{return normSquare()<=vect.normSquare();}

	GVector3 operator +(const GVector3& vect) const;
    GVector3& operator +=(const GVector3& vect);
    GVector3 operator -(const GVector3& vect) const;
	inline GVector3 operator -() const						{GVector3 w(*this); for(int i=0; i<3; ++i) w[i]*=-1; return w;}
	GVector3& operator -=(const GVector3& vect);
	inline Real operator *(const GVector3& vect) const		{return  (_v[0] * vect[0]) + (_v[1] * vect[1]) + (_v[2] * vect[2]);}
    inline GVector3 operator *(Real k) const				{GVector3 w(*this); for(int i=0; i<3; ++i) w[i]*=k; return w;}
    inline GVector3& operator *=(Real k)					{for(int i=0; i<3; ++i) _v[i]*=k; return *this;}
	inline GVector3 operator /(Real k) const				{GVector3 w(*this); for(int i=0; i<3; ++i) w[i]/=k; return w;}
    inline GVector3& operator /=(Real k)					{for(int i=0; i<3; ++i) _v[i]/=k; return *this;}
    GVector3 operator ^(const GVector3& vect) const;
    GVector3& operator ^=(const GVector3& vect);

	//access
    inline Real& operator [](uint index)					{return _v[index];}
    inline const Real& operator [](uint index) const		{return _v[index];}
	inline const Real at(uint index) const					{return _v[index];}
	inline Real* data()										{return &_v[0];}
	inline const Real* const_data() const					{return &_v[0];}
	inline const Real& x() const							{return _v[0];}
	inline const Real& y() const							{return _v[1];}
	inline const Real& z() const							{return _v[2];}
	inline Real& x()										{return _v[0];}
	inline Real& y()										{return _v[1];}
	inline Real& z()										{return _v[2];}
	inline Real norm() const								{return GMath::sqrt( normSquare() );}
	inline Real normSquare() const							{Real n = .0f;for(int i=0; i<3;++i) n+=(_v[i]*_v[i]); return n;}
	inline GVector3 normalized() const
	{
		GVector3 w(*this);
		Real normS = normSquare();
		if (!isZero(normS))
			w /= GMath::sqrt(normS);
		return w;
	}
	inline Real distanceTo(const GVector3& w) const			{GVector3 d = this->operator-(w); return d.norm();}
	Radians angleRadians(const GVector3& w) const;
	//this projected to other (result is parallel to other)
	GVector3 projectedTo(const GVector3& other) const;
	//array must be allocated with new GVector3[3]
	void ortonormalBasis(GVector3* arrayOfVectors) const;
	void clamp(Real min, Real max);
	void clamp(const GVector3& min, const GVector3& max);
	void clamp(const GVector3& min, Real max);
	void clamp(Real min, const GVector3& max);
	inline GString toString() const
	{
		return GString("{ ") +GString::number(_v[0]) +" , " +GString::number(_v[1]) +" , "
							 +GString::number(_v[2]) + " }"    ;
	}
	inline GString toSting_onlyValues() const
	{
		return GString::number(_v[0]) + " , " + GString::number(_v[1]) + " , " + GString::number(_v[2]);
	}
	inline void setValues(Real x, Real y, Real z)	{ _v[0]=x; _v[1]=y; _v[2]=z; }

	bool isNormalized() const						{ return areEqual(this->normSquare(), 1.0f); }
	//true == |all elements|<=epsilon
    bool isNull() const;
    bool isParallelTo(const GVector3& vect) const;
    bool isNormalTo(const GVector3& vect) const;
private:
	Real _v[3];
};


}		//EO namespace GMath
#endif // GVECTOR3_H