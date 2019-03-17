#pragma once
#ifndef GQUATERNION_H
#define GQUATERNION_H

#include "GMatrix4x4.h"
namespace GMath{

// <v, w>
class GQuaternion
{
	inline friend GQuaternion operator*(Real k, const GQuaternion& q)			{return q*k;}
	inline friend std::ostream& operator<<(std::ostream& os,const GQuaternion& q)	
	{
		os << "{ "<<q._w<< " | "<< q._v.x() << " , "<< q._v.y() << " , "<< q._v.z() << " }";return os;	
	}
public:
	GQuaternion(void)											{_w=0.0f;}
	GQuaternion(const GVector3& u, Real w)						{_v=u; _w=w;}
	GQuaternion(const GQuaternion& q)							{_w=q._w; _v=q._v;}
	//vector must be unit length
	GQuaternion(Radians angle, const GVector3& rotationVersor);
	GQuaternion(const GMatrix3x3& M);
	~GQuaternion(void)											{}

	//access
	inline Real* data()											{return _v.data();}
	inline const Real* const_data() const						{return _v.const_data();}
	inline Real& w()											{return _w;}
	inline Real& x()											{return _v.x();}
	inline Real& y()											{return _v.y();}
	inline Real& z()											{return _v.z();}
	inline GVector3& v()										{return _v;}
	inline const Real& w() const								{return _w;}
	inline const Real& x() const								{return _v.x();}
	inline const Real& y() const								{return _v.y();}
	inline const Real& z() const								{return _v.z();}
	inline const GVector3& v() const								{return _v;}
	inline GString toString() const
	{
		return GString("{")+GString::number(_w)+" | "    + GString::number(_v.x())+" , " 
						  + GString::number(_v.y())+" , "+ GString::number(_v.z())+" }";
	}

	inline void setW(Real w)									{_w=w;}
	//versor must be unit length
	inline void setVersor(const GVector3& u)						{_v=u;}
	//versor must be unit length
	inline void setVersor(Real x, Real y , Real z)				{_v.setValues(x,y,z);}

	//operators
	inline GQuaternion& operator=(const GQuaternion& q)			{_w=q._w; _v=q._v; return *this;}
	
	inline GQuaternion operator+(const GQuaternion& p) const		{return GQuaternion(p._v+_v,p._w+_w);}
	inline GQuaternion& operator+=(const GQuaternion& p)			{_v+=p._v; _w+=p._w; return *this;}
	inline GQuaternion operator-(const GQuaternion& p) const		{return GQuaternion(_v-p._v,_w-p._w);}
	inline GQuaternion& operator-=(const GQuaternion& p)			{_v-=p._v; _w-=p._w; return *this;}
	//Grassman product
	GQuaternion operator*(const GQuaternion& p) const;
	GQuaternion& operator*=(const GQuaternion& p);
	inline GQuaternion operator*(Real k) const					{return GQuaternion(_v*k,_w*k);}
	inline GQuaternion& operator*= (Real k)						{_v*=k; _w*=k;return *this;}
	inline GQuaternion operator/(Real k) const					{return GQuaternion(_v/k,_w/k);}
	inline GQuaternion& operator/= (Real k)						{_v/=k; _w/=k;return *this;}

	inline bool operator==(const GQuaternion& p) const			{if( _v==p._v && areEqual(_w,p._w)) return true; return false;}
	inline bool operator!=(const GQuaternion& p) const			{return !operator==(p);}
	inline bool operator>(const GQuaternion& p) const			{return (normSquare()>p.normSquare() );}
	inline bool operator<(const GQuaternion& p) const			{return (normSquare()<p.normSquare() );}

	inline Real normSquare() const								{return _v.normSquare() +(_w*_w);}
	inline Real norm() const									{return GMath::sqrt(normSquare() );}
	inline GQuaternion normalized() const						{Real n=norm(); return GQuaternion(_v/n,_w/n);}
	inline void normalizeMe()									{Real n=norm(); _v/=n; _w/=n;}
	//==inverse if norm==1
	inline GQuaternion conjugate() const						{return GQuaternion(_v*(-1.0),_w );}
	inline void conjugateMe()									{_v=-_v;}
	inline GQuaternion inverse() const							{return (conjugate()/=normSquare());}
	//w= cos(alfa/2)
	inline Radians angle() const								{return arcCos(_w)*2.0f;}
	//the corrispondent rotation Matrix
	GMatrix3x3 rotationMatrix() const;
	void axisAngleEquivalent(Radians& modificableAngle, GVector3& modificableVect) const;

private:
	GVector3 _v;
	Real _w;
};

	//WARNING it assumes unit length quaternion
	void rotateVector(GVector3& v, const GQuaternion& q);
	//makes L == linear interpolation between A and B		//0<=beta<=1
	inline void Lerp(const GQuaternion& A, const GQuaternion& B, Real beta, _Out_ GQuaternion& L)
	{
		beta = ( beta > 1.0f) ? 1.0f : (( beta < 0.0f) ? 0.0f :  beta);  // Clamp value to 0 to 1
		L.x()= (1-beta)*A.x() + beta*B.x();
		L.y()= (1-beta)*A.y() + beta*B.y();
		L.z()= (1-beta)*A.z() + beta*B.z();
		L.w()=(1-beta)*A.w() + beta*B.w();
		L.normalizeMe();
	}
	//linear interpolation
	//lerp(A,B,beta) = [(1-beta)*A+beta*B]normalized		//0<=beta<=1
	inline GQuaternion Lerp(const GQuaternion& A, const GQuaternion& B, Real beta)			{GQuaternion L;Lerp(A,B,beta,L);return L;}
	//Spherical Linear Interpolation
	void SLerp(const GQuaternion& p, const GQuaternion& q, Real beta, _Out_ GQuaternion& SL);
	//Spherical Linear Interpolation
	inline GQuaternion SLerp(const GQuaternion& p, const GQuaternion& q, Real beta)			{GQuaternion S;SLerp(p,q,beta,S);return S;}

}		//EO namespace GMath


#endif // GQUATERNION_H
