#pragma once
#ifndef GTIME_H
#define GTIME_H 

#include "GMathFunctions.h"
#include <GStringList.h>

namespace GMath
{
//-------------------------------------------------------------------------------------------------------
//Class GTime: represent time. Have size of a double floating point. Can store up to 1.967593e+303 days
//-------------------------------------------------------------------------------------------------------
class GTime
{
public:
	GTime(void)						: _secs(0.0f)			{}
	explicit GTime(double seconds)	: _secs(seconds)		{}
	GTime(const GTime& t)			: _secs(t._secs)		{}
	GTime(uint32 h, uint8 m, uint8 s=0, uint16 ms=0);
	virtual ~GTime(void)									{}

	inline GTime addMSecs(Int32 ms) const					{double s = static_cast<double>(ms)/1000.0f; return GTime(_secs+s);}
	inline GTime addSecs(Int32 s) const						{return GTime(_secs+s);}
	inline Int32 hour() const								{return static_cast<Int32>(_secs/3600.0f);}
	Int32 minutes() const;
	Int32 secs() const;
	Int32 msecs() const;
	//24h = 86399.999000 sec
	inline double inSeconds() const							{return _secs;}
	inline Int32 msecsTo(const GTime& t) const				{double d = t._secs-_secs; d*=1000; return static_cast<Int32>(d);}
	inline double secsTo(const GTime& t) const				{return t._secs-_secs;}
	//sets to h,m,s,ms if they are valid return true
	bool setHMS(uint32 h, uint8 m, uint8 s=0, uint16 ms=0);
	inline void setTimeinSeconds(double timeInSeconds)		{_secs=timeInSeconds;}

	GString toString(const GString& separator = " : ") const;

	//operators
	inline GTime& operator=(const GTime& other)				{_secs = other._secs; return *this;}
	inline GTime operator+(const GTime& other) const		{GTime t(_secs+other._secs); return t;}
	inline GTime& operator+=(const GTime& other)			{_secs+=other._secs; return *this;}
	inline GTime operator-(const GTime& other) const		{GTime t(_secs-other._secs); return t;}
	inline GTime& operator-=(const GTime& other)			{_secs-=other._secs; return *this;}
	inline GTime operator*(const Real& x) const				{GTime t(_secs*x); return t;}
	inline GTime& operator*=(const Real& x)					{_secs*=x; return *this;}
	inline GTime operator/(const Real& x) const				{return this->operator*(1.0f/x);}
	inline GTime& operator/=(const Real& x)					{return this->operator*=(1.0f/x);}

	inline bool operator==(const GTime& other) const		{if(areEqual(_secs,other._secs)) return true; return false;}
	inline bool operator!=(const GTime& other) const		{if(!areEqual(_secs,other._secs)) return true; return false;}
	inline bool operator<(const GTime& other) const			{if(_secs<other._secs) return true; return false;}
	inline bool operator<=(const GTime& other) const		{if(_secs<other._secs || areEqual(_secs,other._secs)) return true; return false;}
	inline bool operator>(const GTime& other) const			{if(_secs>other._secs) return true; return false;}
	inline bool operator>=(const GTime& other) const		{if(_secs>other._secs || areEqual(_secs,other._secs)) return true; return false;}

	//static members

	//24h = 86399.999000 sec
	static double timeInSeconds(const GTime& t)				{return t._secs;}
	static GTime currentTime();
	static GTime fromString(const GString& str, const GString& separator=" : ");
	static bool isValid(uint32 h, uint8 m, uint8 s, uint16 ms=0);

private:
	double _secs;
};

//doubles max value is roughly 1,80*(10^308) == 1.967593e+303 days == 6.347065e+301 months

inline std::ostream& operator<<(std::ostream& os,const GMath::GTime& t)		{return operator<<(os,t.toString(":").data());}

}	//EO namespace GMath

#endif // GTIME_H 