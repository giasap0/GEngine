#include "commonHeaders.h"
#include "GTime.h"
using namespace GMath;

GTime::GTime(uint32 h, uint8 m, uint8 s, uint16 ms)
{
	_secs=s;
	_secs+=static_cast<double>(ms)/1000.0f;
	_secs+=60.0f*m;
	_secs+=3600.0f*h;
}

Int32 GTime::minutes() const
{//1h = 3600s
	double h = floor(_secs/3600.0f);
	h*=3600.0f;
	return static_cast<Int32>((_secs-h)/60.0f);
}
Int32 GTime::secs() const
{
	double h = floor(_secs/3600.0f);
	h*=3600.0f;
	double m = floor( (_secs-h)/60.0f );
	m*=60.f;
	return static_cast<Int32>( _secs-h-m );
}
Int32 GTime::msecs() const
{
	double h = floor(_secs/3600.0f);
	h*=3600.0f;
	double m = floor( (_secs-h)/60.0f );
	m*=60.f;
	double sec = floor( _secs-h-m );
	return static_cast<Int32>((_secs-h-m-sec)*1000.0f);
}
////msec (uint16) sec(uint8) min(uint8) 
bool GTime::setHMS(uint32 h, uint8 m, uint8 s, uint16 ms)
{
	bool valid = (m>=0 && m<60) && (h>=0) && (s>=0 && s<60) && (ms>=0 && ms<1000);
	if(!valid)
		return false;
	_secs=s;
	_secs+=static_cast<double>(ms)/1000.0f;
	_secs+=60.0f*m;
	_secs+=3600.0f*h;
	return true;
}

GString GTime::toString(const GString& separator) const
{
	double h = floor(_secs/3600.0f);
	h*=3600.0f;
	double m = floor( (_secs-h)/60.0f );
	m*=60.f;
	double sec = floor( _secs-h-m );
	double ms = floor( (_secs-h-m-sec)*1000.0f );
	
	GString str = GString::number( static_cast<Int32>(h/3600.0f));
	str.append(separator);
	str+= GString::number(static_cast<Int32>( m/60.0f ) ) + separator + GString::number(static_cast<Int32>(sec));
	str += separator + GString::number(static_cast<Int32>(ms));
	return str;
}

													//statics

GTime GTime::currentTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	return GTime(st.wHour,static_cast<GMath::uint8>(st.wMinute),static_cast<GMath::uint8>(st.wSecond),st.wMilliseconds);
}

GTime GTime::fromString(const GString& str, const GString& separator)
{
	GStringList terms = str.split(separator);
	uint h = terms.at(0).toUint();
	uint m = terms.at(1).toUint();
	uint se = terms.at(2).toUint();
	uint ms = terms.at(3).toUint();
	return GTime(h,m,se,ms);
}

bool GTime::isValid(uint32 h, uint8 m, uint8 s, uint16 ms)
{
	bool valid = (m>=0 && m<60) && (h>=0) && (s>=0 && s<60) && (ms>=0 && ms<1000);
	return valid;
}
