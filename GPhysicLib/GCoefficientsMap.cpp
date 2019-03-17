#include "GCoefficientsMap.h"

using namespace GPhysics;
using namespace GAereodynamic;

GCoefficientsMap::GCoefficientsMap(uint size)
{
	_size = size;
	if (_size == 0)
	{
		_angles =  nullptr;
		_coefficients = nullptr;
		return;
	}
	_angles = new Radians[size];
	_coefficients = new Coefficients[size];
}


GCoefficientsMap::~GCoefficientsMap()
{
	if (_angles != nullptr)
	{
		delete[] _angles;
		_angles = nullptr;
	}
	if (_coefficients != nullptr)
	{
		delete[] _coefficients;
		_coefficients = nullptr;
	}
}


GCoefficientsMap::GCoefficientsMap(const GCoefficientsMap& map)
{
	_size = map._size;
	if (_size == 0)
	{
		_angles = nullptr;
		_coefficients = nullptr;
		return;
	}
	_angles = new Radians[_size];
	_coefficients = new Coefficients[_size];
	for (uint i = 0; i < _size; i++)
	{
		_angles[i] = map._angles[i];
		_coefficients[i] = map._coefficients[i];
	}
}


GCoefficientsMap& GCoefficientsMap::operator=(const GCoefficientsMap& map)
{
	if (_size == 0 && map._size == 0)	
		return *this;
	if (_angles != nullptr)
	{
		delete[] _angles;
		_angles = nullptr;
	}
	if (_coefficients != nullptr)
	{
		delete[] _coefficients;
		_coefficients = nullptr;
	}

	_size = map._size;
	if (_size == 0 )
		return *this;
	
	_angles = new Radians[_size];
	_coefficients = new Coefficients[_size];
	for (uint i = 0; i < _size; i++)
	{
		_angles[i] = map._angles[i];
		_coefficients[i] = map._coefficients[i];
	}
	return *this;
}
