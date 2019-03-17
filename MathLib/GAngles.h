#pragma once
#ifndef GANGLES_H
#define GANGLES_H 

#include "GMathCore.h"

namespace GMath
{
class Degree;

class Radians
{
    friend class Degree;
public:
	friend inline std::ostream& operator<<(std::ostream& os, const Radians& r)		{return os.operator<<(r._value);}
	friend inline std::istream& operator>>(std::istream& is, Radians& r)			{return is.operator>>(r._value);}
	friend inline bool isNull(Radians x)											{return  isNull(x._value);}
	friend Radians absV(const Radians& r);
	
public:
    explicit Radians(Real angle=0.0f)	:_value(angle)        {}
    Radians(const Radians& other)		:_value(other._value) {}
    Radians(const Degree& degree);

	inline Real toReal() const								{return _value;}

    inline Radians& operator=(const Radians& other)
    {
        if(*this==other) return *this;
        _value = other._value; return *this;
    }
    Radians& operator=(const Degree& degree);
    inline Radians& operator=(const Real& other)			{_value = other; return *this;}

    inline Radians operator+(const Radians& other) const   {return Radians(_value+other._value);}
    inline Radians operator+(const Degree& other) const    {return this->operator +(Radians(other));}
    inline Radians operator+(const Real& other) const      {return Radians(_value+other);}

    inline Radians& operator+=(const Radians& other)       {_value+=other._value; return *this;}
    inline Radians& operator+=(const Degree& other)        {return this->operator +=(Radians(other));}
    inline Radians& operator+=(const Real& other)          {_value+=other; return *this;}

    inline Radians operator-() const                       {return Radians(-_value);}
    inline Radians operator-(const Radians& other) const   {return Radians(_value-other._value);}
    inline Radians operator-(const Degree& other) const    {return this->operator -(Radians(other));}
    inline Radians operator-(const Real& other) const      {return Radians(_value-other);}

    inline Radians& operator-=(const Radians& other)       {_value-=other._value; return *this;}
    inline Radians& operator-=(const Degree& other)        {return this->operator -=(Radians(other));}
    inline Radians& operator-=(const Real& other)          {_value-=other; return *this;}

    inline Radians operator*(const Radians& other) const   {return Radians(_value*other._value);}
    inline Radians operator*(const Degree& other) const    {return this->operator *(Radians(other));}
    inline Radians operator*(const Real& other) const      {return Radians(_value*other);}

    inline Radians& operator*=(const Radians& other)       {_value*=other._value; return *this;}
    inline Radians& operator*=(const Degree& other)        {return this->operator *=(Radians(other));}
    inline Radians& operator*=(const Real& other)          {_value*=other; return *this;}

    inline Radians operator/(const Radians& other) const   {return Radians(_value/other._value);}
    inline Radians operator/(const Degree& other) const    {return this->operator /(Radians(other));}
    inline Radians operator/(const Real& other) const      {return Radians(_value/other);}

    inline Radians& operator/=(const Radians& other)       {_value/=other._value; return *this;}
    inline Radians& operator/=(const Degree& other)        {return this->operator /=(Radians(other));}
    inline Radians& operator/=(const Real& other)          {_value/=other; return *this;}


    bool operator ==(const Radians& other) const;
    bool operator ==(const Degree& other) const;
    bool operator ==(const Real& other) const;

    inline bool operator !=(const Radians& other) const    {return !operator==(other);}
    inline bool operator !=(const Degree& other) const     {return !operator==(other);}
    inline bool operator !=(const Real& other) const       {return !operator==(other);}

    inline bool operator <(const Radians& other) const     {if(_value<other._value) return true; return false;}
    inline bool operator <(const Degree& other) const      {return this->operator <(Radians(other));}
    inline bool operator <(const Real& other) const        {if(_value<other) return true; return false;}

    inline bool operator <=(const Radians& other) const    {if(_value<=other._value) return true; return false;}
    inline bool operator <=(const Degree& other) const     {return this->operator <=(Radians(other));}
    inline bool operator <=(const Real& other) const       {if(_value<=other) return true; return false;}

    inline bool operator >(const Radians& other) const     {if(_value>other._value) return true; return false;}
    inline bool operator >(const Degree& other) const      {return this->operator >(Radians(other));}
    inline bool operator >(const Real& other) const        {if(_value>other) return true; return false;}

    inline bool operator >=(const Radians& other) const	   {if(_value>=other._value)	return true; return false;}
    inline bool operator >=(const Degree& other) const     {return this->operator >=(Radians(other));}
    inline bool operator >=(const Real& other) const       {if(_value>=other) return true; return false;}

private:
    Real _value;
	static const Real _conversion;
};

class Degree
{
    friend class Radians;
	friend inline std::ostream& operator<<(std::ostream& os, const Degree& d)		{return os.operator<<(d._value);}
	friend inline std::istream& operator>>(std::istream& is, Degree& d)				{return is.operator>>(d._value);}
	friend inline bool isNull(Degree x)												{return isNull(x._value);}
	friend Degree absV(const Degree& d);
	
public:
    explicit Degree(Real angle= 0.0f)			: _value(angle)         {}
    Degree(const Degree& other)					: _value(other._value)  {}
	Degree(const Radians& radians)										{_value = radians._value*_conversion;}

	inline Real toReal() const											{return _value;}

    inline Degree& operator=(const Degree& other)
    {
        if(*this==other)        return *this;
        _value = other._value;  return *this;
    }
	inline Degree& operator=(const Radians& other)			{_value = other._value*_conversion; return *this;}
    inline Degree& operator=(const Real& other)				{_value = other; return *this;}

    inline Degree operator+(const Degree& other) const     {return Degree(_value+other._value);}
    inline Degree operator+(const Radians& other) const    {return this->operator +(Degree(other));}
    inline Degree operator+(const Real& other) const       {return Degree(_value+other);}

    inline Degree& operator+=(const Degree& other)         {_value+=other._value; return *this;}
    inline Degree& operator+=(const Radians& other)        {return this->operator +=(Degree(other));}
    inline Degree& operator+=(const Real& other)           {_value+=other; return *this;}

    inline Degree operator-() const                        {return Degree(-_value);}
    inline Degree operator-(const Degree& other) const     {return Degree(_value-other._value);}
    inline Degree operator-(const Radians& other) const    {return this->operator -(Degree(other));}
    inline Degree operator-(const Real& other) const       {return Degree(_value-other);}

    inline Degree& operator-=(const Degree& other)         {_value-=other._value; return *this;}
    inline Degree& operator-=(const Radians& other)        {return this->operator -=(Degree(other));}
    inline Degree& operator-=(const Real& other)           {_value-=other; return *this;}

    inline Degree operator*(const Degree& other) const     {return Degree(_value*other._value);}
    inline Degree operator*(const Radians& other) const    {return this->operator *(Degree(other));}
    inline Degree operator*(const Real& other) const       {return Degree(_value*other);}

    inline Degree& operator*=(const Degree& other)         {_value*=other._value; return *this;}
    inline Degree& operator*=(const Radians& other)        {return this->operator *=(Degree(other));}
    inline Degree& operator*=(const Real& other)           {_value*=other; return *this;}

    inline Degree operator/(const Degree& other) const     {return Degree(_value/other._value);}
    inline Degree operator/(const Radians& other) const    {return this->operator /(Degree(other));}
    inline Degree operator/(const Real& other) const       {return Degree(_value/other);}

    inline Degree& operator/=(const Degree& other)         {_value/=other._value; return *this;}
    inline Degree& operator/=(const Radians& other)        {return this->operator /=(Degree(other));}
    inline Degree& operator/=(const Real& other)           {_value/=other; return *this;}

    bool operator==(const Degree& other) const;
    bool operator==(const Radians& other) const;
    bool operator==(const Real& other) const;

	inline bool operator!=(const Degree& other) const      {return !operator==(other);}
    inline bool operator!=(const Radians& other) const     {return !operator==(other);}
    inline bool operator!=(const Real& other) const        {return !operator==(other);}

    inline bool operator<(const Degree& other) const       {if(_value<other._value) return true; return false;}
    inline bool operator<(const Radians& other) const      {return this->operator <(Degree(other));}
    inline bool operator<(const Real& other) const         {if(_value<other) return true; return false;}

    inline bool operator<=(const Degree& other) const      {if(_value<=other._value) return true; return false;}
    inline bool operator<=(const Radians& other) const     {return this->operator <=(Degree(other));}
    inline bool operator<=(const Real& other) const        {if(_value<=other) return true; return false;}

    inline bool operator>(const Degree& other) const       {if(_value>other._value) return true; return false;}
    inline bool operator>(const Radians& other) const      {return this->operator >(Degree(other));}
    inline bool operator>(const Real& other) const         {if(_value>other) return true; return false;}

    inline bool operator>=(const Degree& other) const      {if(_value>=other._value) return true; return false;}
    inline bool operator>=(const Radians& other) const     {return this->operator >=(Degree(other));}
    inline bool operator>=(const Real& other) const        {if(_value>=other) return true; return false;}

private:
    Real _value;
	static const Real _conversion;
};

}	//EO namespace GMath

#endif // GANGLES_H