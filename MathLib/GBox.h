#pragma once
#ifndef GBOX_H
#define GBOX_H 

#include "GRay.h"

/*********************
**	   y^			**
**		|			**
**		|			**
**	z(x)|----->x	**
**********************/

namespace GMath{
enum class GBoxPlanes : uint8{
	Invalid=0, Front=1, Back, Base, Top, Left, Right
};
GBoxPlanes& operator++( GBoxPlanes& p)
{
	switch (p)
	{
	case GMath::GBoxPlanes::Invalid: return p;
		break;
	case GMath::GBoxPlanes::Front: p= GBoxPlanes::Back; return p;
		break;
	case GMath::GBoxPlanes::Back: p= GBoxPlanes::Base; return p;
		break;
	case GMath::GBoxPlanes::Base: p= GBoxPlanes::Top; return p;
		break;
	case GMath::GBoxPlanes::Top: p= GBoxPlanes::Left; return p;
		break;
	case GMath::GBoxPlanes::Left: p= GBoxPlanes::Right; return p;
		break;
	case GMath::GBoxPlanes::Right: p= GBoxPlanes::Invalid; return p;
		break;
	default: p= GBoxPlanes::Invalid; return p;
		break;
	}
	return p;
};

class GBox
{
public:
	GBox(void);
	GBox(const GPoint& center, Real side);
	GBox(const GPoint& center, Real xRadius, Real yRadius, Real zRadius);
	~GBox(void)																					{}

	GPoint&			center()								{return _center;}
	const GPoint&	center() const							{return _center;}
	inline Real xRadius() const								{return _xRadius;}
	inline Real yRadius() const								{return _yRadius;}
	inline Real zRadius() const								{return _zRadius;}
	inline Real volume() const								{return _xRadius*_yRadius*_zRadius*8.0f;}
	inline Real baseArea() const							{return _xRadius*_zRadius*4.0f;}
	GPlane getPlane( GBoxPlanes select ) const;
	bool isPointInside(const GPoint& point) const;
	bool intersect(const GRay& ray, GPoint& intersectionPoint) const;

private:
	GPoint _center;
	Real _xRadius, _yRadius, _zRadius;
};

}//EO namespace GMath
#endif	//EO GBOX_H 