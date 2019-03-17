//#include "commonHeaders.h"
#include "GAngles.h"

using namespace GMath;

const Real Radians::_conversion = 3.14159265358979323846f / 180.0f;
const Real Degree::_conversion = 180.0f / 3.14159265358979323846f;

const Radians Radians::pi = Radians( castReal<float>(3.14159265358979323846f) );
const Radians Radians::pi_half = Radians::pi / 2.0f;
const Radians Radians::pi_double = Radians::pi*2.0f;
const Radians Radians::pi_quarter = Radians::pi / 4.0f;

GMath::Radians GMath::absV(const GMath::Radians& alfa)
{
	return Radians(GMath::absV(alfa._value));
}

Degree GMath::absV(const Degree& d)
{
	return Degree( GMath::absV(d._value) );
}

Radians::Radians(const Degree &degree)
{
	_value = degree._value*_conversion;
}

Radians& Radians::operator =(const Degree& degree)
{
	_value = degree._value*_conversion;
    return *this;
}

bool Radians::operator==(const Radians& other) const
{
	if(areEqual(_value,other._value))
		return true;
	return false;
}
bool Radians::operator==(const Degree& other) const
{
	return this->operator ==(Radians(other));
}
bool Radians::operator==(const Real& other) const
{
	if(areEqual(_value,other))
		return true;
	return false;
}
bool Degree::operator==(const Degree& other) const
{
	if(areEqual(_value,other._value))
		return true;
	return false;
}
bool Degree::operator==(const Radians& other) const
{
	return this->operator ==(Degree(other));
}
bool Degree::operator==(const Real& other) const
{
	if(areEqual(_value,other))
		return true;
	return false;
}
