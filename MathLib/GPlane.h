#pragma once
#ifndef GPLANE_H
#define GPLANE_H

#include "GVector4.h"
#include "GPoint.h"

namespace GMath
{
//--------------------------------------------------------------------------------------------------
//Class GPlane: represent a plane in 3D [ax+by+cz+d=0]
//--------------------------------------------------------------------------------------------------
class GPlane
{
	friend GPlane operator *(const Real& d, const GPlane& plane);
public:
	GPlane(void)							:_v()			{}
	GPlane(Real x, Real y, Real z, Real w)	:_v(x,y,z,w)	{}
	GPlane(const GVector4& fromVect)		:_v(fromVect)	{}
	//Construct a plane through a normal, and a point 
	GPlane(const GVector3& normal, const GPoint& point);
	//Construct a plane through a point and 2 direction vectors
	GPlane(const GPoint& point, const GVector3& vector1, const GVector3& vector2);
	//Construct a plane through 3 points
	GPlane(const GPoint& point1, const GPoint& point2, const GPoint& point3);
	GPlane(const GPlane& other)			:_v(other._v)	{}
	virtual ~GPlane(void)								{}

	inline bool operator ==(const GPlane& p) const		{return (_v==p._v);}
	inline bool operator !=(const GPlane& p) const		{return (_v!=p._v);}

	inline GPlane& operator=(const GPlane& other)		{_v=other._v; return *this;}
	inline GPlane operator *(Real k) const				{return GPlane(_v*k);}
	inline GPlane& operator *=(Real k)					{_v*=k; return *this;}
	inline GPlane operator /(Real k) const				{return GPlane(_v/k);}
	inline GPlane& operator /=(Real k)					{_v/=k; return *this;}

	//access
	inline const Real& a() const						{return _v[0];}
	inline Real& a()									{return _v[0];}
	inline const Real& b() const						{return _v[1];}
	inline Real& b()									{return _v[1];}
	inline const Real& c() const						{return _v[2];}
	inline Real& c()									{return _v[2];}
	inline const Real& d() const						{return _v[3];}
	inline Real& d()									{return _v[3];}
	//return a reference to a vector with plane parameters as elements
	inline const GVector4& vector() const				{return _v;}

	inline GVector3 normalVector() const				{return GVector3(_v[0],_v[1],_v[2]);}
	//project a vector onto the plane
	GVector3 projectVector(const GVector3& v) const;
	//true if point belong to this plane
	bool belongToPlain(const GPoint& point) const;
	bool isParallelTo(const GPlane& p) const			{return normalVector().isParallelTo( p.normalVector() );}
	//minimum distance from plane to point
	Real distanceToPoint(const GPoint& point) const;
	//minimum distance from plane to point - can be negative depending on normal versor
	Real distanceToPointRelative(const GPoint& point) const;
	//Normalizes the plane coefficients so that the plane normal has unit length
	inline GPlane normalizedPlane() const				{GPlane copy(*this); copy.normalize(); return copy;}
	//Normalizes the plane coefficients so that the plane normal has unit length
	void normalize();

	//TO DO
	static GPoint intersectPlanes(const GPlane& p1, const GPlane& p2, const GPlane& p3);

private:
	GVector4 _v;
};

}		//EO namespace GMath

#endif		//GPLANE_H