#include "GRenderIncludes.h"
#include "GCamera.h"

using namespace GMath;

GCamera::GCamera(void)
{
	_position.x() = _position.y() = _position.z() = 0.0f;
}

void GCamera::setPosition(const GPoint& newPos)
{
	_position.x() = newPos.x();
	_position.x() = newPos.y();
	_position.x() = newPos.z();
}

void GCamera::render()
{
	GVector3 up(0,1,0);
	GVector3 lookAt(0,0,1);

	GMatrix4x4 rotationMatrix( GMath::GMatrix3x3::RotationMatrix(_rotX,_rotY,_rotZ) );
	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	up =up*rotationMatrix;
	lookAt = lookAt*rotationMatrix;

	// Translate the rotated camera
	lookAt+= _position;

	// create the view matrix and store values
	_viewMatrix = GMatrix4x4::LookAtLH(_position,lookAt,up);
}
