#pragma once

#ifndef GMATH_FUNCTIONS_H
#define GMATH_FUNCTIONS_H

#include "GAngles.h"

/*****************************************
  * This contains usefull                *
  * declarations, constants and funtions *
  ****************************************/

namespace GMath
{

//natural logarithm
inline Real ln(Real value)										{return log(value);}
//logarithm base 2
inline Real log_2(Real x)										{return static_cast<Real> (log(x)/log(2));}
inline Real log_base(Real x, Real base)							{return log(x)/log(base);}
inline Real pow(const Real& x, const Real& exponent)			{return powf(x,exponent);}
inline Real pow_int(const Real& x, Int16 exponent)				{return _Pow_int<Real>(x,exponent);}
//return x^2
inline Real pow2(const Real& x)									{return x*x;}
inline Real sqrt(const Real& x)									{return sqrtf(x);}
inline Int32 factorial(Int32 value)
{
	if(value<=1)
		return 1;
	Int32 fatt=1;
	for(Int32 i=2; i<=value; ++i)
	{
		fatt*=i;
	}
	return fatt;
}
//a  fast reciprocal  square root approximation
inline Real reciprocalSqrt( Real x )
{ 
    long i; 
    Real y, r; 
    y = x * 0.5f; 
    i = *(long *)( &x ); 
    i = 0x5f3759df - ( i >> 1 ); 
    r = *(Real *)( &i ); 
    r = r * ( 1.5f - r * r * y ); 
    return r; 
} 

inline bool operator ==(const Real& x ,const Radians& angle)	{return angle==x;}
inline bool operator !=(const Real& x ,const Radians& angle)	{return angle!=x;}
inline bool operator <(const Real& x ,const Radians& angle)		{return angle<x;}
inline bool operator <=(const Real& x ,const Radians& angle)	{return angle<=x;}
inline bool operator >(const Real& x ,const Radians& angle)		{return angle>x;}
inline bool operator >=(const Real& x ,const Radians& angle)	{return angle>=x;}

inline bool operator ==(const Real& x ,const Degree& angle)		{return angle==x;}
inline bool operator !=(const Real& x ,const Degree& angle)		{return angle!=x;}
inline bool operator <(const Real& x ,const Degree& angle)		{return angle<x;}
inline bool operator <=(const Real& x ,const Degree& angle)		{return angle<=x;}
inline bool operator >(const Real& x ,const Degree& angle)		{return angle>x;}
inline bool operator >=(const Real& x ,const Degree& angle)		{return angle>=x;}

inline Real sin(const Radians& angle)							{return sinf(angle.toReal()); }
inline Real sin(const Degree& angle)							{Radians x(angle);	return sinf(x.toReal());}
inline Radians arcSin(const Real& val)							{return Radians(asin(val));}

inline Real cos(const Radians& angle)							{return cosf(angle.toReal()); }
inline Real cos(const Degree& angle)							{Radians x(angle);	return cosf(x.toReal());}
inline Radians arcCos(const Real& val)							{return Radians(acos(val));}

inline Real tan(const Radians& angle)							{return tanf(angle.toReal()); }
inline Real tan(const Degree& angle)							{Radians x(angle);	return tanf(x.toReal());}
inline Radians arcTan(const Real& val)							{return Radians(atan(val));}

//makes |angle| <=2pi
inline Radians reduceAngle(Radians alfa)	
{
	while(GMath::absV(alfa)>=Constants::pi_double)
	{
		if(alfa>0.0f)
			alfa-=Constants::pi_double;
		else
			alfa+=Constants::pi_double;
	}
	return alfa;
}
//makes |angle| <=360°
inline Degree reduceAngle(Degree alfa)	
{
	while(GMath::absV(alfa)>=360.0f)
	{
		if(alfa>0.0f)
			alfa-=360.0f;
		else
			alfa+=360.0f;
	}
	return alfa;
}

//return maximum between x-y
template<class T> inline T maximum(const T& x, const T& y)						{if(x>=y) return x; return y;}
template<class T> inline T maximum(const T& x, const T& y, const T& z)			{T t= maximum(x,y); return maximum(t,z);}

//return minimum between x-y
template<class T> inline T minimum(const T& x, const T& y)						{if(x<=y) return x; return y;}
template<class T> inline T minimum(const T& x, const T& y, const T& z)			{T t = minimum(x,y); return minimum(t,z);}

//clamp x to (min-max) min&max included
inline void clamp(Real& x, Real min, Real max)									{if(x<min+Constants::approssimation) x=min; else if(x>max-Constants::approssimation) x=max; return;}
template<class T> inline void clamp(T& x, const T& min, const T& max)			{if(x<min) x=min; else if(x>max) x=max;	return;}
template<class T> inline void clampMin(T& x, const T& min)						{if(x<min) x=min; return;}
inline void clampMin(Real& x, Real min)											{if(x< (min+Constants::approssimation) ) x=min; return;}
template<class T> inline void clampMax(T& x, const T& max)						{if(x>max) x=max; return;}
inline void clampMax(Real& x, Real max)											{if(x > (max- Constants::approssimation) ) x=max; return;}
//check if x<min && x>max
inline bool checkClamp(Real x, Real min, Real max)								{if(x<min+Constants::approssimation) return false; if(x>max-Constants::approssimation) return false; return true;}
template<class T> inline bool checkClamp(const T&x, const T&min, const T&max)	{if(x<min) return false; if(x>max) return false; return true;}

//linear interpolation
//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
inline Real lerp(Real x, Real y, Real beta)										{ clamp(beta, .0f, 1.0f); return (((1.0f - beta)*x) + (beta*y)); }

}	//	EO	namespace GMath

#endif // MATHFUNCTIONS_H



//{return ( x > max) ? max : (( x < min) ? min :  x);  }
