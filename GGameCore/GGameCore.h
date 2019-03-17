#pragma once
#ifndef GGAMECORE_H
#define GGAMECORE_H

#include "Entities\GCharacter.h"

//--------------------------------------------------------------------------------------
//Class GGameLogic: manage the logic, implement rules of the game
//--------------------------------------------------------------------------------------
class GGameLogic
{
public:
	GGameLogic()					{}
	~GGameLogic()					{}

	bool initialize();
	void shutDown();

	void updateState(Real dt);

private:

private:
	//prevent copy
	GGameLogic( const GGameLogic&);
	GGameLogic& operator=(const GGameLogic&);
};

#endif		//GGAMECORE_H