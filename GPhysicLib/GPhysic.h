#pragma once
#ifndef GPHYSIC_H
#define GPHYSIC_H

#include "GPhysicIncludes.h"
#include "GAirplane.h"
#include "GAirplaneLoader.h"

namespace GPhysics
{
	//--------------------------------------------------------------------------------------
	//Class GPhysic: manage the physic system
	//--------------------------------------------------------------------------------------
	class GPhysic
	{
	public:
		GPhysic()		{}
		~GPhysic()		{}

		bool initialize();
		void shutDown();

		inline const GAirplaneStatus& getPlaneStatus()	const					{ return _pAirPlane->planeStatus(); }
		inline GAirplaneStatus& getPlaneStatus()								{ return _pAirPlane->planeStatus(); }
		void updateState(Real dt, const GPlaneCommand& planeCommand);

	private:
		GPhysics::GAirplane* _pAirPlane;

	private:
		//prevent copy
		GPhysic(const GPhysic&);
		GPhysic& operator=(const GPhysic&);
	};
}

#endif		//GPHYSIC_H