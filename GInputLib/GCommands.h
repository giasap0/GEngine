#pragma once
#ifndef GCOMMANDS_H
#define GCOMMANDS_H

class GCommands
{
public:

	GCommands()								{ }
	virtual ~GCommands()					{ }

private:
	float _cameraLeft, _cameraRight, _cameraUp, _cameraDown;
};

#endif	//GCOMMANDS_H