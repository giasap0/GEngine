#include "GShapes.h"
using namespace GMath;
using namespace GShapes;

GBox::GBox(void)	:_center(), _xRadius(0.0f), _yRadius(0.0f), _zRadius(0.0f)
{
}
GBox::GBox(const GPoint& center, Real side)		:_center(), _xRadius(side/2.0f), _yRadius(side/2.0f), _zRadius(side/2.0f)
{
}
GBox::GBox(const GPoint& center, Real xRadius, Real yRadius, Real zRadius)	:_center(), _xRadius(xRadius), _yRadius(yRadius), _zRadius(zRadius)
{
}

GPlane GBox::getPlane( GBoxPlanes select ) const
{
	//I build plane passing for 3 vertex of the box
	GPoint v1, v2, v3;
	switch (select)
	{
	case GBoxPlanes::Front: 
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		break;
	case GBoxPlanes::Back:
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() + _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() + _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() + _zRadius );
		break;
	case GBoxPlanes::Base:
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() + _zRadius );
		break;
	case GBoxPlanes::Top:
		v1.setPosition( _center.x() -_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() + _zRadius );
		break;
	case GBoxPlanes::Left:
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() -_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() -_xRadius , _center.y() +_yRadius, _center.z() + _zRadius );
		break;
	case GBoxPlanes::Right:
		v1.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() + _zRadius );
		break;
	default:	return GPlane();
		break;
	}
	return GPlane( v1,v2,v3);
}

bool GBox::isPointInside( const GPoint& point) const
{
	bool condX = (point.x() >= (_center.x()-_xRadius) ) && (point.x() <= (_center.x() +_xRadius) );
	bool condY = (point.y() >= (_center.y()-_yRadius) ) && (point.y() <= (_center.y() +_yRadius) );
	bool condZ = (point.z() >= (_center.z()-_zRadius) ) && (point.z() <= (_center.z() +_zRadius) );
	if( condX && condY && condZ )
		return true;
	else
		return false;
}

bool GBox::intersect(const GBox& other) const
{
	bool condX = ( (other._center.x() + other._xRadius )>= (_center.x()-_xRadius) ) && ( (other._center.x() - other._xRadius) <= (_center.x() +_xRadius) );
	bool condY = ( (other._center.y() + other._yRadius )>= (_center.y()-_yRadius) ) && ( (other._center.y() - other._yRadius) <= (_center.y() +_yRadius) );
	bool condZ = ( (other._center.z() + other._zRadius )>= (_center.z()-_zRadius) ) && ( (other._center.z() - other._zRadius) <= (_center.z() +_zRadius) );
	if( condX && condY && condZ )
		return true;
	else
		return false;
}

bool GBox::intersect(const GRay& ray, GPoint& intersectionPoint) const
{
	GPlane currentPlane;
	for(GBoxPlanes p= GBoxPlanes::Front; p!= GBoxPlanes::Invalid; ++p)
	{
		currentPlane = getPlane( p );
		if( ! ray.isParallelTo(currentPlane) )	//==ray intersect plane
		{
			intersectionPoint = ray.intersect( currentPlane );
			if( isPointInside( intersectionPoint ) )
				return true;
		}
	}
	return false;
}


GSphere::GSphere(Real radius)	: _center(), _radius( radius)
{
}
GSphere::GSphere(const GPoint& center, Real radius)	: _center( center), _radius(radius)
{
}

bool GSphere::isPointInside(const GPoint& point) const
{
	Real d = _center.distanceTo(point);
	if(d<=_radius)
		return true;
	return false;
}
bool GSphere::intersect(const GSphere& other) const
{
	Real d = _center.distanceTo( other._center );
	Real sum = _radius+ other._radius;
	if( d<= sum)
		return true;
	else
		return false;
}

bool intersection(const GSphere& s, const GBox& box)
{
	Real d = s.center().distanceTo( box.center() );
	bool condX = (s.center().x() >= box.center().x()-box.xRadius()-s.radius()) && ( s.center().x() <= box.center().x()+box.xRadius()+s.radius() );
	bool condY = (s.center().y() >= box.center().y()-box.yRadius()-s.radius()) && ( s.center().y() <= box.center().y()+box.yRadius()+s.radius() );
	bool condZ = (s.center().z() >= box.center().z()-box.zRadius()-s.radius()) && ( s.center().z() <= box.center().z()+box.zRadius()+s.radius() );
	if( condX && condY && condZ )
		return true;
	else
		return false;
}
