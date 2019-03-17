#include "GBox.h"
using namespace GMath;

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
	GPoint v1, v2, v3;
	switch (select)
	{
	case GMath::GBoxPlanes::Front: 
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		break;
	case GMath::GBoxPlanes::Back:
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() + _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() + _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() + _zRadius );
		break;
	case GMath::GBoxPlanes::Base:
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() -_yRadius, _center.z() + _zRadius );
		break;
	case GMath::GBoxPlanes::Top:
		v1.setPosition( _center.x() -_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() +_xRadius , _center.y() +_yRadius, _center.z() + _zRadius );
		break;
	case GMath::GBoxPlanes::Left:
		v1.setPosition( _center.x() -_xRadius , _center.y() -_yRadius, _center.z() - _zRadius );
		v2.setPosition( _center.x() -_xRadius , _center.y() +_yRadius, _center.z() - _zRadius );
		v3.setPosition( _center.x() -_xRadius , _center.y() +_yRadius, _center.z() + _zRadius );
		break;
	case GMath::GBoxPlanes::Right:
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
