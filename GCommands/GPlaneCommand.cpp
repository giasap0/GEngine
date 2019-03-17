#include "GPlaneCommand.h"
#include <GMath.h>

using GMath::areEqual;

GPlaneCommand::GPlaneCommand(const GPlaneCommand& other)
{
	_throttle = other._throttle;
	_roll = other._roll;
	_yaw = other._yaw;
	_pitch = other._pitch;
}

bool GPlaneCommand::operator == (const GPlaneCommand& other) const
{
	return (areEqual(_throttle, other._throttle) && areEqual(_roll, other._roll) && areEqual(_yaw, other._yaw) && areEqual(_pitch, other._pitch));
}