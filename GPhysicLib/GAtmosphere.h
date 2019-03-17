#pragma once
#ifndef GATMOSPHERE_H
#define GATMOSPHERE_H
#include "GPhysicConstants.h"

namespace GPhysics
{
	class GAtmosphere
	{
	public:
		virtual ~GAtmosphere() { }

		inline static Real seaLevelDensity()					{ return GPhysicConstants::ro0;  }
		//altitude in meters
		inline static Real getAirDensity(Real altitude)
		{ 
			if (altitude <= .0f + GMath::Constants::approssimation )
				return seaLevelDensity();

		//	return GPhysicConstants::ro0 - (altitude*ro_rate*GPhysicConstants::ro0); //modello lineare (approssimazione di aria standard)
			if (altitude <= 11000.0f + GMath::Constants::approssimation )
				return GPhysicConstants::ro0*GMath::pow( ((288.0f - (altitude*0.0065f)) / 288.0f ), 4.25f);
			else
				return 0.3639f * GMath::pow(GMath::Constants::euler, (-9.81f* (altitude - 11000.0f)) / 62243.545f );
		}

		//ro/ro0
		//altitude in meters
		inline static Real getAirDensityRatio(Real altitude)
		{
			if (altitude <= .0f +GMath::Constants::approssimation  )
				return 1.0f;
			//return 1.0f - (altitude*ro_rate);
			return GPhysicConstants::ro0 / getAirDensity(altitude);
		}

	private:
		static const Real ro_rate;
	private:
		GAtmosphere();
		GAtmosphere(const GAtmosphere&);
		GAtmosphere& operator =(const GAtmosphere& other);
	};

}

#endif	//GATMOSPHERE_H