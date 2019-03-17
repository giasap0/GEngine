#include "GAirplaneBuildInfo.h"
#include "GPhysicIncludes.h"
using namespace GPhysics;
using namespace GMath;

GAirplaneBuildInfo::GAirplaneBuildInfo(const GAirplaneBuildInfo& a) : _coefficientsMap(0)
{
	_max_power = a._max_power;
	_surface = a._surface;
	_wingSpan = a._wingSpan;
	_chord = a._chord;
	_mass = a._mass;
	_inertia = a._inertia;
	_inertiaInverse = a._inertiaInverse;
	_coefficientsMap = a._coefficientsMap;
	_deflection_limit_elevator = a._deflection_limit_elevator;
	_deflection_limit_aileron = a._deflection_limit_aileron;
	_deflection_limit_rudder = a._deflection_limit_rudder;
}

GAirplaneBuildInfo& GAirplaneBuildInfo::operator=(const GAirplaneBuildInfo& a)
{
	_max_power = a._max_power;
	_surface = a._surface;
	_wingSpan = a._wingSpan;
	_chord = a._chord;
	_mass = a._mass;
	_inertia = a._inertia;
	_inertiaInverse = a._inertiaInverse;
	_coefficientsMap = a._coefficientsMap;
	_deflection_limit_elevator = a._deflection_limit_elevator;
	_deflection_limit_aileron = a._deflection_limit_aileron;
	_deflection_limit_rudder = a._deflection_limit_rudder;
	return *this;
}
