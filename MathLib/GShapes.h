#pragma once
#ifndef GSHAPES_H
#define GSHAPES_H 

#include "GRay.h"

/*********************
**	   y^			**
**		|			**
**		|			**
**	z(x)|----->x	**
**********************/

namespace GMath{
	namespace GShapes{

enum class GBoxPlanes : uint8{
	Invalid=0, Front=1, Back, Base, Top, Left, Right
};
GBoxPlanes& operator++( GBoxPlanes& p)
{
	switch (p)
	{ 
	case GBoxPlanes::Invalid: return p;
		break;
	case GBoxPlanes::Front: p= GBoxPlanes::Back; return p;
		break;
	case GBoxPlanes::Back: p= GBoxPlanes::Base; return p;
		break;
	case GBoxPlanes::Base: p= GBoxPlanes::Top; return p;
		break;
	case GBoxPlanes::Top: p= GBoxPlanes::Left; return p;
		break;
	case GBoxPlanes::Left: p= GBoxPlanes::Right; return p;
		break;
	case GBoxPlanes::Right: p= GBoxPlanes::Invalid; return p;
		break;
	default: p= GBoxPlanes::Invalid; return p;
		break;
	}
	return p;
};

//-------------------------------------------
//class GBox : represent an axis-oriented Box 
//-------------------------------------------
class GBox
{
public:
	GBox(void);
	GBox(const GPoint& center, Real side);
	GBox(const GPoint& center, Real xRadius, Real yRadius, Real zRadius);
	~GBox(void)												{}

	inline GPoint& center()									{return _center;}
	inline const GPoint& center() const						{return _center;}
	inline Real xRadius() const								{return _xRadius;}
	inline Real yRadius() const								{return _yRadius;}
	inline Real zRadius() const								{return _zRadius;}
	inline Real& xRadius()									{return _xRadius;}
	inline Real& yRadius()									{return _yRadius;}
	inline Real& zRadius()									{return _zRadius;}
	inline Real volume() const								{return _xRadius*_yRadius*_zRadius*8.0f;}
	inline Real baseArea() const							{return _xRadius*_zRadius*4.0f;}
	GPlane getPlane( GBoxPlanes select ) const;
	bool isPointInside(const GPoint& point) const;
	bool intersect(const GBox& other) const;
	bool intersect(const GRay& ray, GPoint& intersectionPoint) const;

private:
	GPoint _center;
	Real _xRadius, _yRadius, _zRadius;
};

//--------------------------------------------------------------
//class GSphere : represent a Sphere with a center and a radius
//--------------------------------------------------------------
class GSphere
{
public:
	explicit GSphere(Real radius= 1.0f);
	GSphere(const GPoint& center, Real radius);
	~GSphere()												{}

	inline GPoint& center()									{return _center;}
	inline const GPoint& center() const						{return _center;}
	inline Real radius() const								{return _radius;}
	inline Real& radius()									{return _radius;}
	inline Real volume() const								{return 4.0f*pow_int(_radius,3)*Constants::pi/3.0f;}
	inline Real surface() const								{return 4.0f*pow2(_radius)*Constants::pi;}
	bool isPointInside(const GPoint& point) const;
	bool intersect(const GSphere& other) const;

private:
	GPoint _center;
	Real _radius;
};

bool intersection(const GSphere& s, const GBox& box);

}	//EO namespace GShapes
}	//EO namespace GMath

#endif	//EO GSHAPES_H 