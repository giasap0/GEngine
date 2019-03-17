#include "GCameraCommand.h"
#include <GMath.h>

using GMath::areEqual;


bool GCameraCommand::operator == (const GCameraCommand& other) const
{
	return (areEqual(cameraHorizontal, other.cameraHorizontal) && areEqual(cameraVertical, other.cameraVertical));
}