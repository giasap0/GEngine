#pragma once
#ifndef GVECTOR4_H
#define GVECTOR4_H

#include "GVector3.h"

namespace GMath
{
//sizeof float[4]
class GVector4
{
	//linear interpolation
	//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
	inline friend GVector4 Lerp(const GVector4& A, const GVector4& B, Real beta)
	{
		beta = ( beta > 1.0f) ? 1.0f : (( beta < 0.0f) ? 0.0f :  beta);  // Clamp value to 0 to 1
		return ( ((1.0f-beta)*A ) + (beta*B) );
	}
	inline friend GVector4 SmoothStep(const GVector4& A, const GVector4& B, Real t)
	{
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t*t*(3.f - (2.f*t) );
		return Lerp(A,B,t);
	}
	inline friend GVector4 CatmullRom(const GVector4& pos0, const GVector4& pos1, const GVector4& pos2, const GVector4& pos3, Real t)
	{
		float t2 = t*t;
		float t3 = t2*t;
		float P0 = -t3 + 2.0f * t2 - t;								float P1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
		float P2 = -3.0f * t3 + 4.0f * t2 + t;						float P3 = t3 - t2;
		GVector4 result;
		result[0] = (P0 * pos0.x() + P1 * pos1.x() + P2 * pos2.x() + P3 * pos3.x() ) * 0.5f;
		result[1] = (P0 * pos0.y() + P1 * pos1.y() + P2 * pos2.y() + P3 * pos3.y() ) * 0.5f;
		result[2] = (P0 * pos0.z() + P1 * pos1.z() + P2 * pos2.z() + P3 * pos3.z() ) * 0.5f;
		result[3] = (P0 * pos0.w() + P1 * pos1.w() + P2 * pos2.w() + P3 * pos3.w() ) * 0.5f;
		return result;
	}

	inline friend std::ostream& operator<<(std::ostream& os,const GVector4& c)
	{
		os <<"{" <<c._v[0] << ", "<< c._v[1] << ", "<< c._v[2] << ", "<< c._v[3] << " }";
		return os;
	}
	friend GVector4 operator *(const Real& d, const GVector4& vect);
public:
	GVector4(void)											{_v[0]=0; _v[1]=0; _v[2]=0; _v[3]=0;}
	GVector4(Real x, Real y, Real z, Real w=0.0f)			{ _v[0]=x; _v[1]=y; _v[2]=z; _v[3]=w; }
	explicit GVector4(const Real v[4])						{ _v[0]=v[0];  _v[1]=v[1];  _v[2]=v[2];  _v[3]=v[3]; }
	explicit GVector4(const GVector3& v)					{_v[0]=v.x(); _v[1]=v.y(); _v[2]=v.z(); _v[3]=0;}
	GVector4(const Real& norm, const GVector4& parallelTo);
	GVector4(const GVector4& other)							{_v[0]=other._v[0]; _v[1]=other._v[1]; 
															 _v[2]=other._v[2]; _v[3]=other._v[3];}
	~GVector4(void)											{}

	GVector4& operator=(const GVector4& other)				{_v[0]=other._v[0]; _v[1]=other._v[1];
															_v[2]=other._v[2]; _v[3]=other._v[3]; 
															return *this;}
//they compares all elements
	bool operator ==(const GVector4& vect) const;         
	inline bool operator !=(const GVector4& vect) const		{return !this->operator==(vect);}
//they compares norms
	inline bool operator  >(const GVector4& vect) const		{return normSquare()>vect.normSquare();}
    inline bool operator  <(const GVector4& vect) const		{return normSquare()<vect.normSquare();}
    inline bool operator >=(const GVector4& vect) const		{return normSquare()>=vect.normSquare();}
    inline bool operator <=(const GVector4& vect) const		{return normSquare()<=vect.normSquare();}

	GVector4 operator +(const GVector4& vect) const;
    GVector4& operator +=(const GVector4& vect);
    GVector4 operator -(const GVector4& vect) const;
	inline GVector4 operator -() const						{GVector4 w(*this); for(int i=0; i<4; ++i) w[i]*=-1; return w;}
	GVector4& operator -=(const GVector4& vect);
    Real operator *(const GVector4& vect) const;
	inline Real operator*(const GVector3& w) const			{return this->operator*( GVector4(w) );}
    inline GVector4 operator *(Real k) const				{GVector4 w(*this); for(int i=0; i<4; ++i) w[i]*=k; return w;}
    inline GVector4& operator *=(Real k)					{for(int i=0; i<4; ++i) _v[i]*=k; return *this;}
	inline GVector4 operator /(Real k) const				{GVector4 w(*this); for(int i=0; i<4; ++i) w[i]/=k; return w;}
    inline GVector4& operator /=(Real k)					{for(int i=0; i<4; ++i) _v[i]/=k; return *this;}
	//acts like 3vectors
    GVector4 operator ^(const GVector4& vect) const;
    GVector4& operator ^=(const GVector4& vect);

	//access
    Real& operator [](uint index)							{return _v[index];}
    const Real& operator [](uint index) const				{return _v[index];}
	const Real& at(uint index) const						{return _v[index];}
	inline Real* data()										{return &_v[0];}
	inline const Real* const_data() const					{return &_v[0];}
	inline const Real& x() const							{return _v[0];}
	inline const Real& y() const							{return _v[1];}
	inline const Real& z() const							{return _v[2];}
	inline const Real& w() const							{return _v[3];}
	inline Real& x()										{return _v[0];}
	inline Real& y()										{return _v[1];}
	inline Real& z()										{return _v[2];}
	inline Real& w()										{return _v[3];}

	inline Real norm() const								{return GMath::sqrt( normSquare() );}
	inline Real normSquare() const							{Real n = .0f;for(int i=0; i<4;++i) n+=(_v[i]*_v[i]); return n;}
	inline GVector4 normalized() const
	{
		GVector4 w(*this);
		Real normS = normSquare();
		if ( !isZero(normS))
			w /= GMath::sqrt(normS);
		return w;
	}
	inline Real distanceTo(const GVector4& w) const			{GVector4 d = this->operator-(w); return d.norm();}
	inline GVector3 get3DVect() const						{return GVector3(_v[0],_v[1],_v[2]);}
	Radians angleRadians(const GVector4& w) const;
	GVector4 projectedTo(const GVector4& other) const;
	//array must be allocated with new GVector4[3]
	void ortonormalBasis(GVector4* arrayOfVectors) const;
	void clamp(Real min, Real max);
	void clamp(const GVector4& min, const GVector4& max);
	void clamp(const GVector4& min, Real max);
	void clamp(Real min, const GVector4& max);
	inline GString toString() const
	{
		return GString("{ ") +GString::number(_v[0]) +" , " +GString::number(_v[1]) +" , "
							 +GString::number(_v[2]) +" , " +GString::number(_v[3]) + " }"    ;
	}

	inline void setValues(Real x, Real y, Real z, Real w=0.0f)	{ _v[0]=x; _v[1]=y; _v[2]=z; _v[3]=w; }
	inline void setW(Real w)									{_v[3]=w;}
	//true == |all elements|<=epsilon
    bool isNull() const;
    bool isParallelTo(const GVector4& vect) const;
    bool isNormalTo(const GVector4& vect) const;
private:
	Real _v[4];
};

}		//EO namespace GMath
#endif // GVECTOR4_H



