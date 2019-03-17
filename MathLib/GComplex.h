#pragma once
#ifndef COMPLEX_H
#define COMPLEX_H

#include "GMathFunctions.h"
#include <GVector.h>
#include <GStringList.h>

namespace GMath
{

class GComplex
{
	inline friend std::ostream& operator<<(std::ostream& os,const GComplex& c)
	{
		if(c._im>=0.0f)
			os << c._r << " + i "<< c._im;
		else
			os << c._r << " - i "<< -c._im;
		return os;
	}
	friend GComplex	pow(const GComplex& c, const Real& exponent);
	friend GComplex	e_powToComplex(const GComplex& exponent);
	friend GComplex  ln(const GComplex& c);
	friend GComplex	sqRoot(const GComplex& c);
	friend GComplex	sqRoot(Real d);
	friend GVector<GComplex> nRoot(const GComplex& c, int n);
    friend GComplex	operator +(const Real& a, const GComplex& b);
    friend GComplex	operator -(const Real& a, const GComplex& b);
    friend GComplex	operator *(const Real& a, const GComplex& b);
    friend GComplex	operator /(const Real& a, const GComplex& b);
    friend bool		operator ==(const Real& a, const GComplex& b);
    friend bool		operator !=(const Real&a, const GComplex& b);

public:
	GComplex(const Real &real=0.0, const Real &img=0.0)	:_r(real), _im(img)		{}
	GComplex(const Real& length, const Radians& alfa);
	GComplex(const GComplex& complex)								:_r(complex._r), _im(complex._im) {}
	~GComplex(void)	{}

	inline Real real() const										{return _r;}
	inline Real img() const											{return _im;}
	inline Real norm() const										{return sqrt(pow2(_r)+pow2(_im) );}
	inline Real normSquare() const									{return pow2(_r)+pow2(_im);}
	Radians angleRadians() const;
	Degree angleDegree() const;
	GString toString() const;
    GString toStringEuler(bool angleInRadians = true) const;
	bool fromString(const GString& str, const GString& format="a + i b");

	inline void setReal(const Real& newReal)						{_r=newReal;}
	inline void setImg(const Real& newImg)							{_im=newImg;}
	inline void setValues(const Real& real, const Real& img)		{_r=real; _im=img;}
    void setValues(const Real& length, const Radians& alfa);

	inline GComplex conjugate() const								{return GComplex(_r, -_im);}
    GComplex reciprocal() const;
    GComplex exponentiation(const Real &exponent) const;

	inline GComplex& operator =(const GComplex& c)					{_r=c._r; _im=c._im; return *this;}
	inline GComplex& operator =(const Real& d)						{_r=d; _im=0.0f; return *this;}
	inline GComplex operator +(const GComplex& c) const				{GComplex z(*this); z._r+=c._r; z._im+=c._im; return z;}
    inline GComplex operator +(const Real& d) const					{GComplex z(*this); z._r+=d; return z;}
    inline GComplex& operator +=(const GComplex& c)					{_r+=c._r; _im+=c._im; return *this;}
	inline GComplex& operator +=(const Real& d)						{_r+=d; return *this;}
    inline GComplex operator -(const GComplex& c) const				{GComplex z(*this); z._r-=c._r; z._im-=c._im; return z;}
    inline GComplex operator -(const Real& d) const					{GComplex z(*this); z._r-=d; return z;}
	inline GComplex operator -() const								{GComplex c(*this); c._r= -c._r; c._im = -c._im; return c;}
    inline GComplex& operator -=(const GComplex& c)					{_r-=c._r; _im-=c._im; return *this;}
    inline GComplex& operator -=(const Real& d)						{_r-=d; return *this;}
    GComplex operator *(const GComplex& c) const;
	inline GComplex operator *(const Real& d) const					{return GComplex(_r*d,_im*d);}
    GComplex& operator *=(const GComplex& c);
	inline GComplex& operator *=(const Real& d)						{_r*=d;_im*=d; return *this;}
	inline GComplex operator /(const GComplex& c) const				{GComplex inv = GMath::pow(c,-1); return this->operator *(inv);}
	inline GComplex operator /(const Real& d) const					{return GComplex(_r/d,_im/d);}
    inline GComplex& operator /=(const GComplex& c)					{GComplex inv = GMath::pow(c,-1); return this->operator*=(inv);}
    inline GComplex& operator /=(const Real& d)						{_r/=d;_im/=d; return *this;}

	bool operator ==(const GComplex& c) const;
    bool operator ==(const Real& d) const;
    bool operator !=(const GComplex& c) const;
    bool operator !=(const Real& d) const;
    bool operator >(const GComplex& c) const;
    bool operator <(const GComplex& c) const;
    bool operator >=(const GComplex& c) const;
    bool operator <=(const GComplex& c) const;

	inline bool isReal() const										{if( areEqual(_im,0.0f) ) return true; return false;}

private:
	Real _r, _im;
};


}		//EO namespace GMath
#endif // COMPLEX_H

/**
public:
    void add(const Real& real, const Real& img=0.0);


};

#endif // COMPLEX_H


GComplex ln(GComplex c);
QVector<GComplex> nroot(GComplex c, int n);
*/