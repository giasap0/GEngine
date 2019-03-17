#pragma once
#ifndef GMATH_CORE_H
#define GMATH_CORE_H 

#include <iostream>
#include <stdint.h>
#include <math.h>

/*****************************************************
**		   Min			 Max		 Type		Bit	**
**		2.2e-308		1.79e+308	double		64	**
**		1.1e-38			3.4e+38		float		32	**
**			0			 +255		uint8_t		8	**
**		  -128		     +127		int8_t		8	**
**			0			+65,535		uint16_t	16	**	
**		-32,678			+32,767		int16_t		16	**	
**			0		+4,294,967,295	uint32_t	32	**
**	-2,147,483,648	+2,147,483,647	int32_t		32	**		//==int
******************************************************/

namespace GMath
{
typedef uint64_t uint64;
typedef int64_t  Int64;
typedef uint32_t uint32;
typedef int32_t  Int32;
typedef uint16_t uint16;
typedef int16_t  Int16;
typedef uint8_t	 uint8;
typedef int8_t   Int8;
// Real type (float)
typedef float Real;
//cast only numbers! (int, float, double, ecc)
template <typename T> inline Real castReal(const T& a){return Real(static_cast<float>(a));}

//my constants
class Constants{
public:
	//3.14159265358979323846
    static const Real pi;
	static const Real pi_half;
	static const Real pi_double;
	static const Real pi_quarter;
	//2.71828182845904523536
    static const Real euler;
	//(float minimum) * 10 (==around 1.1*10^-37)
    static const Real epsilon;
	//hard coded as 10^-10
    static const Real approssimation;

	//other mathematical constants

	static const Real millsConstant;

	//min and max

	static const double doubleMin;
	static const Real realMin;
	static const Int64 int64Min;
	static const Int32 int32Min;
	static const Int16 int16Min;
	static const Int8  int8Min;

	static const double doubleMax;
	static const Real realMax;
	static const Int64 int64Max;
	static const Int32 int32Max;
	static const Int16 int16Max;
	static const Int8  int8Max;
	static const uint64 uint64Max;
	static const uint32 uint32Max;
	static const uint16 uint16Max;
	static const uint8 uint8Max;

private:        //do not define them!
    Constants();
    Constants(const Constants& o);
    Constants& operator =(const Constants& other);
};

inline Real absV(Real x)
{
	if( (x-Constants::approssimation < 0.0f) && (x+Constants::approssimation > 0.0f) )
		return 0.0f;
	if(x+Constants::approssimation < 0.0f)
		return -x;
	else
		return x;
}

//compare abs(x-y)<approssimation
inline bool areEqual(Real x, Real y)
{
    if( (x>y-Constants::approssimation) && (x<y+Constants::approssimation) )
        return true;
    return false;
}

//true if |x|<approssimation
inline bool isNull(Real x)
{
	if( (x> (-Constants::approssimation) ) && (x<Constants::approssimation) )
        return true;
    return false;
}

inline bool areEqual(double x, double y)
{
	if( (x>y-Constants::approssimation) && (x<y+Constants::approssimation) )
		return true;
	return false;
}
inline bool areEqual(Real x, Real y, Real epsilon)
{
	if( (x>y-epsilon) && (x<y+epsilon) )
        return true;
    return false;
}

}	//	EO	namespace GMath

#endif // GMATH_CORE_H