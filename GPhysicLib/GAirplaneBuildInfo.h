#pragma once
#ifndef GAIRPLANE_BUILDING_INFO_H
#define GAIRPLANE_BUILDING_INFO_H

#include <GMath.h>
#include "GAereodynamic.h"

using namespace GMath;
using GPhysics::GAereodynamic::GCoefficientsMap;

namespace GPhysics
{
	class GAirplaneLoader;

	class GAirplaneBuildInfo
	{
		friend class GAirplaneLoader;
	public:
		GAirplaneBuildInfo()	: _coefficientsMap(0)			{ }
		virtual ~GAirplaneBuildInfo()							{ }
		GAirplaneBuildInfo(const GAirplaneBuildInfo&);
		GAirplaneBuildInfo& operator= (const GAirplaneBuildInfo&);

		// access - read only

		const Real& max_power() const    						{ return _max_power; }
		const Real& surface() const    							{ return _surface; }
		const Real& wingSpan() const    						{ return _wingSpan; }
		const Real& chord() const    							{ return _chord; }
		const Real& mass() const    							{ return _mass; }
		const GMatrix3x3& inertia() const						{ return _inertia; }
		const GMatrix3x3& inertiaInverse() const				{ return _inertiaInverse; }
		const GCoefficientsMap& coefficientsMap() const			{ return _coefficientsMap; }

		const Radians& deflection_limit_elevator() const		{ return _deflection_limit_elevator; }
		const Radians& deflection_limit_aileron() const			{ return _deflection_limit_aileron; }
		const Radians& deflection_limit_rudder() const			{ return _deflection_limit_rudder; }

	private:
		Real _max_power;
		Real _surface;
		Real _wingSpan;
		Real _chord;
		Real _mass;
		GMatrix3x3 _inertia;
		GMatrix3x3 _inertiaInverse;
		GCoefficientsMap _coefficientsMap;

		Radians _deflection_limit_elevator, _deflection_limit_aileron, _deflection_limit_rudder;
	private:
		
	};
}//EO namespace
#endif			//GAIRPLANE_BUILDING_INFO_H