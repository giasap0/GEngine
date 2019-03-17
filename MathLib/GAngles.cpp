//#include "commonHeaders.h"
#include "GAngles.h"

using namespace GMath;

const Real Radians::_conversion = Constants::pi/180.0f;
const Real Degree::_conversion = 180.0f/Constants::pi;


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
