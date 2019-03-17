#pragma once
#ifndef GLIGHT_H
#define GLIGHT_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <DirectXMath.h>
#include <GMath.h>
#include "Shaders\GColors.h"
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;
using GMath::GVector3;

//--------------------------------------------------------------------------------------
//Class : GLight handle direction and color of lights
//--------------------------------------------------------------------------------------
class GLight
{
public:
	GLight(void)				: _direction()							{_ambientColor = _diffuseColor = GColors::White;}
	GLight(const GLight& l)												{_ambientColor = l._ambientColor; _diffuseColor=l._diffuseColor; _direction=l._direction;}
	~GLight(void)														{}

	inline void setAmbientColor(float red, float green, float blue, float alpha)
	{
		_ambientColor.red=red;	_ambientColor.green=green;	_ambientColor.blue=blue;	_ambientColor.alpha=alpha;	
	}
	inline void setAmbientColor(const GColor& RGBA)					{_ambientColor= RGBA;}

	inline void setDiffuseColor(float red, float green, float blue, float alpha)
	{
		_diffuseColor.red=red;	_diffuseColor.green=green;	_diffuseColor.blue=blue;	_diffuseColor.alpha=alpha;	
	}
	inline void setDiffuseColor(const GColor& RGBA)					{_diffuseColor= RGBA;}

	inline void setDirection(float x, float y, float z)				{ _direction.setValues(x,y,z);	}
	inline void setDirection(const GVector3& u)						{_direction=u;}

	inline const GColor& getAmbientColor() const							{return _ambientColor;}
	inline const GColor& getDiffuseColor() const							{return _diffuseColor;}
	inline const GVector3& getDirection() const							{return _direction;}

private:
	GColor _ambientColor;
	GColor _diffuseColor;
	GVector3 _direction;
};

#endif	//GLIGHT_H
