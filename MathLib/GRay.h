#pragma once
#ifndef GRAY_H
#define GRAY_H

#include "GPlane.h"
#include "GMatrix3x3.h"

namespace GMath
{
//------------------------------------------------------------------------------------------------------------------
//Class GRay: represent a ray (or a line) in 3D like a direction (unit lenght) and a point (starting point) [6 floats]
//------------------------------------------------------------------------------------------------------------------
class GRay
{
public:
	GRay(void)										: _dir(),_p()							{}
	GRay(const GVector3& parallel, const GPoint& P)	: _dir(parallel.normalized()), _p(P)	{}
	GRay(const GRay& parallel, const GPoint& P)		: _dir(parallel._dir), _p(P)			{}
	//from two distinct intersecting planes
	GRay(const GPlane& p1, const GPlane& p2);
	//ray through points A and B
	GRay(const GPoint& A, const GPoint& B);
	GRay(const GRay& copy)							:_dir(copy._dir),_p(copy._p)			{}
	virtual ~GRay(void)												{}

	//are coincident
	inline bool operator==(const GRay& r) const						{if( isParallelTo(r) ) return belongToRay(r._p); return false;}
	inline bool operator!=(const GRay& r) const						{return !this->operator==(r);}

	//access

	//direction is unit lenght
	inline GVector3 direction() const								{return _dir;}
	inline GPoint	startPoint() const								{return _p;}

	inline GRay& operator=(const GRay& r)							{_dir=r._dir; _p=r._p; return *this;}
	//move ray parallel to himself through direction for distance
	inline void move(const GVector3& direction, Real distance)		{_p.move(distance,direction);}
	inline void rotate(const GMatrix3x3& rotMatrix)					{_dir=_dir*rotMatrix; _dir=_dir.normalized();}

	//true if point belong to this ray
	bool belongToRay(const GPoint& point) const;
	inline bool isParallelTo(const GRay& r) const					{if( _dir== r._dir || _dir==-r._dir) return true; return false;}
	inline bool isParallelTo(const GVector3& dir) const				{if( _dir.isParallelTo(dir) ) return true; return false; }
	inline bool isParallelTo(const GPlane& p) const					{if(_dir.isNormalTo( p.normalVector()) ) return true; return false; }
	inline bool isNormalTo(const GRay& r) const						{return _dir.isNormalTo(r._dir);}
	inline bool isNormalTo(const GVector3& dir) const				{return _dir.isNormalTo(dir);}
	inline bool isNormalTo(const GPlane& p) const					{return _dir.isParallelTo( p.normalVector() );}
	
	// == if not parallel  have intersection point
	inline bool isComplanarTo(const GRay& r) const					{GPlane p(_p,_dir,r._dir); if(p.belongToPlain(r._p))return true; return false;}
	//minimum distance from ray to point
	Real distanceToPoint(const GPoint& point) const;
	//point of intersection (check isComplanar to know if intersection exists)
	GPoint intersect(const GRay& r) const;
	GPoint intersect(const GPlane& p) const;

private:
	GVector3 _dir;
	GPoint _p;
};

}		//EO namespace GMath

#endif	//GRAY_H
