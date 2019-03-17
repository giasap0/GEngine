#pragma once
#ifndef GPHYSIC_H
#define GPHYSIC_H

#include "GPhysicIncludes.h"

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

	void updateState(Real dt);

private:

private:
	//prevent copy
	GPhysic( const GPhysic&);
	GPhysic& operator=(const GPhysic&);
};


#endif		//GPHYSIC_H