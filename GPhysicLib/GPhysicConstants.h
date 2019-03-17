#pragma once
#ifndef GPHYSIC_CONSTANTS_H
#define GPHYSIC_CONSTANTS_H

#include "GPhysicIncludes.h"

namespace GPhysics
{
	class GPhysicConstants
	{
	public:

		static const Real g;
		//air density
		static const Real ro0;
		//sound speed sea level
		static const Real cSound;
		//1 atm in Pascal
		static const Real pATM;

	private:	//do not define them!
		GPhysicConstants();
		GPhysicConstants(const GPhysicConstants&);
		GPhysicConstants& operator =(const GPhysicConstants& other);
	};
}

#endif		//GPHYSIC_CONSTANTS_H
