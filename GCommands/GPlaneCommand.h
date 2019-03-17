#pragma once
#ifndef GPLANECOMMAND_H
#define GPLANECOMMAND_H

/***********************************************************************
		z - yaw (rudder)
		^
		|
		|
		|
		|
		(x)-----------------> y - Pitch (elevator)

		x == Roll (aileron)
***********************************************************************/
class GPlaneCommand
{
public:
	GPlaneCommand()				{}
	GPlaneCommand(const GPlaneCommand& o);
	virtual ~GPlaneCommand()	{}

	bool operator == (const GPlaneCommand& other) const;

	inline GPlaneCommand& operator=(const GPlaneCommand& other)
	{
		if (*this == other) return *this;
		_throttle = other._throttle;
		_roll = other._roll;
		_yaw = other._yaw;
		_pitch = other._pitch;
		return *this;
	}

	inline void resetValues()							{ _throttle = _roll = _yaw = _pitch = .0f; _analogicCommand = false; }

	inline const float& aroundX() const					{ return _roll; }
	inline const float& aroundY() const					{ return _pitch; }
	inline const float& aroundZ() const					{ return _yaw; }

	inline const float& roll() const					{ return _roll; }
	inline const float& yaw() const						{ return _pitch; }
	inline const float& pitch() const					{ return _yaw; }
	inline const float& throttle() const				{ return _throttle; }
	inline const float& trimElevator() const			{ return _trimElevator; }
	inline const bool& isAnalogicCommand() const		{ return _analogicCommand;  }

	inline float& roll() 								{ return _roll; }
	inline float& yaw() 								{ return _pitch; }
	inline float& pitch() 								{ return _yaw; }
	inline float& throttle()							{ return _throttle; }
	inline float& trimElevator()						{ return _trimElevator; }
	inline bool& isAnalogicCommand()					{ return _analogicCommand; }
	
private:
	float _roll, _yaw, _pitch;
	float _throttle;
	float _trimElevator;
	bool _analogicCommand;
};

#endif	//GPLANECOMMAND_H