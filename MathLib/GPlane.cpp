#include "GPlane.h"
#include <GEngineCore.h>
#include <Windows.h>			//needed for debug
using namespace GMath;

GPlane GMath::operator*(const Real& d, const GPlane& plane)		{	return plane*d;	}

//Construct a plane through a normal, and a point 
GPlane::GPlane(const GVector3& normal, const GPoint& point)
{
	if (normal.isNull())
	{
		DBG_OUTPT("GPlane constructor - normal vector is null");
		return;
	}
	//nx(x-x0) + ny(y-y0) +nz(z-z0) = 0
	_v[0]=normal[0];
	_v[1]=normal[1];
	_v[2]=normal[2];
	_v[3]=(_v[0] * point.x() ) +(_v[1] * point.y() ) +(_v[2] * point.z() );
}
//Construct a plane through a point and 2 direction vectors
GPlane::GPlane(const GPoint& point, const GVector3& vector1, const GVector3& vector2)
{
	GVector3 normal = vector1^vector2;
	this->operator=(GPlane(normal,point) );
}
//Construct a plane through 3 points
GPlane::GPlane(const GPoint& point1, const GPoint& point2, const GPoint& point3)
{
	GVector3 normal = -(point2-point1)^(point3-point1);
	this->operator=(GPlane(normal,point1) );
}

//project a vector onto the plane
GVector3 GPlane::projectVector(const GVector3& v) const
{
	GVector3 n(_v[0],_v[1],_v[2]);
	n= n.normalized();
	return v- (v*n)*n;
}
//true if point belong to this plane
bool GPlane::belongToPlain(const GPoint& point) const
{
	if ( areEqual(distanceToPoint(point),0.0f))
		return true;
	return false;
}
//minimum distance from plane to point
Real GPlane::distanceToPoint(const GPoint& point) const
{
	Real numerator = (_v[0]*point.x()) +(_v[1]*point.y()) + (_v[2]*point.z()) + _v[4];
	Real denom = (_v[0]*_v[0]) + (_v[1]*_v[1]) +(_v[2]*_v[2]);
	if( areEqual( denom, 0.0f) )
		DBG_OUTPT("GPlane::distanceTo - division by '0")
	return GMath::absV( numerator ) * GMath::reciprocalSqrt(denom );
}
//minimum distance from plane to point - can be negative depending on normal versor
Real GPlane::distanceToPointRelative(const GPoint& point) const
{
	GVector4 p4( point.getPosition() );
	p4.setW(1.0f);
	Real numerator = _v*p4;
	Real denom = (_v[0]*_v[0]) + (_v[1]*_v[1]) +(_v[2]*_v[2]); 
	if( areEqual( denom, 0.0f) )
		DBG_OUTPT("GPlane::distanceTo(relative) - division by '0");
	return numerator * GMath::reciprocalSqrt(denom);
}

void GPlane::normalize()
{
	GVector3 n(_v[0],_v[1],_v[2]);
	if(n.isNull() )
		DBG_OUTPT("GPlane::normalizeMe() - n is null: division by 0");
	Real d = n.norm();
	if( areEqual(d,1.0f) )
		return;
	_v/=d;
}
