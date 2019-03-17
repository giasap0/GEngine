#include "Engine.h"

using namespace GPhysics;
using GMath::areEqual;

Engine::Engine(const Engine& o)
{
	_max_power = o._max_power;
	_throttle = o._throttle;
}

Engine& Engine::operator=(const Engine& e)
{
	_max_power = e._max_power;
	_throttle = e._throttle;
	return *this;
}

bool Engine::operator==(const Engine& e) const
{
	return  areEqual(_max_power, e._max_power) && areEqual(_throttle , e._throttle);
}
