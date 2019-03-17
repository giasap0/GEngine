#pragma once
#ifndef GCAMERA_H
#define GCAMERA_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <DirectXMath.h>
#include <GMath.h>

//--------------------------------------------------------------------------------------
//Class : GCamera
//--------------------------------------------------------------------------------------
using GMath::Radians;
using GMath::GVector3;
using GMath::GPoint;
using GMath::GMatrix4x4;

//class GCamera, represent the point of view
class GCamera
{
public:
	GCamera(void);
	GCamera(const GCamera&)												{}
	~GCamera(void)														{}

	void setPosition(const GPoint& newPos);
	void setPosition(const GVector3& newPos)							{_position=newPos;}
	inline void setPosition(float x, float y, float z)					{_position.x()=x;_position.y()=y;_position.z()=z;}
	inline void setRotation(Radians rotX, Radians rotY, Radians rotZ)	{_rotX=rotX; _rotY=rotY; _rotZ=rotZ;}	

	inline void move(const GVector3& delta)								{_position+=delta;}
	inline void move(float x, float y, float z)							{_position+= GVector3(x,y,z);}
	inline void applyRotation(Radians rotX, Radians rotY, Radians rotZ)	{ _rotX += rotX; _rotY += rotY; _rotZ += rotZ; }

	inline const GVector3& getPosition() const							{return _position;}
	inline GVector3 getRotation() const
	{
		return GVector3(_rotX.toReal(), _rotY.toReal(), _rotZ.toReal());
	}

	void render();
	inline const GMatrix4x4& getViewMatrix() const						{return _viewMatrix;}

	inline const Radians& rotX() const									{ return _rotX; }
	inline const Radians& rotY() const									{ return _rotY; }
	inline const Radians& rotZ() const									{ return _rotZ; }
	inline Radians& rotX()											{ return _rotX; }
	inline Radians& rotY()											{ return _rotY; }
	inline Radians& rotZ()											{ return _rotZ; }

private:
	GMatrix4x4 _viewMatrix;
	GVector3 _position;
	Radians _rotX, _rotY, _rotZ;
};

#endif	//GCAMERA_H