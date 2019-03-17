#pragma once
#ifndef PLANEENGINE_H
#define PLANEENGINE_H
#include "GPhysicIncludes.h"
#include "GAtmosphere.h"

namespace GPhysics
{
	//--------------------------------------------------------------------------------------
	//Class Engine: represent the airplane engine
	//--------------------------------------------------------------------------------------
	class Engine
	{
	public:
		Engine(void)					{ _max_power = _throttle = .0f; }
		Engine(const Engine& other);
		~Engine(void)					{}

		Engine& operator= (const Engine& e);
		bool operator==(const Engine& e) const;
		inline bool operator!=(const Engine& e) const		{ return !this->operator==(e);  }

		inline Real max_thrust() const							{ return _max_power;  }	//sea level, 1 m/s
		inline Real& max_power()								{ if (_max_power < .0f) _max_power = .0f; return _max_power; }
		inline Real throttle() const							{ return _throttle; }
		inline Real& throttle()
		{
			if (_throttle < .0f) _throttle = .0f;
			else if (_throttle > 1.0f) _throttle = 1.0f;
			return _throttle;
		}

		//altitude in meters, speed in m/s positive
		inline Real thrust(Real altitude, Real speed)
		{
			if( speed < 1.0f )	speed = 1.0f;
			if (_throttle < .0f) _throttle = .0f;
			else if (_throttle > 1.0f) _throttle = 1.0f;

			return _max_power*_throttle* GAtmosphere::getAirDensityRatio(altitude)/speed;
		}

	private:
		Real _max_power, _throttle;
	};

}//EO namespace

#endif	//PLANEENGINE_H
