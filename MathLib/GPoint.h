#pragma once
#ifndef GPOINT_H
#define GPOINT_H

#include "GVector3.h"

namespace GMath
{
class GPoint
{
public:
	GPoint(void)							:_p()					{}
	GPoint(Real x, Real y, Real z)			:_p(x,y,z)				{}
	explicit GPoint(const GVector3& fromVect)		:_p(fromVect)	{}
	GPoint(const GPoint& other)				:_p(other._p)			{}
	virtual ~GPoint(void)											{}

	inline GPoint& operator=(const GPoint& p)						{_p=p._p; return *this;}
	inline bool operator ==(const GPoint& p) const					{return (_p==p._p);}
	inline bool operator !=(const GPoint& p) const					{return (_p!=p._p);}

	inline GVector3 operator-(const GPoint& p) const				{return _p-p._p;}

	inline Real x() const											{return _p.x();}
	inline Real& x()												{return _p[0];}
	inline Real y() const											{return _p.y();}
	inline Real& y()												{return _p[1];}
	inline Real z() const											{return _p.z();}
	inline Real& z()												{return _p[2];}

	inline GString toString() const									{return _p.toString();}

	inline Real distanceTo(const GPoint& p) const					{return (_p-p._p).norm();}
	inline void setPosition(Real x, Real y, Real z)					{_p[0]=x; _p[1]=y; _p[2]=z;}
	inline void setPosition(const GPoint& other)					{_p[0]=other.x(); _p[1]=other.y(); _p[2]=other.z();}
	inline void setPosition(const GVector3& vec)					{_p=vec;}
	inline const GVector3& getPosition() const						{return _p;}
	inline GVector3& getPosition()									{ return _p; }
	inline void move(Real distance, const GVector3& direction)		{_p+= (direction.normalized()*distance);}
	//warning!!! GVector3 MUST be a versor (norm==1)
	inline void moveThroughVersor(Real distance, const GVector3& dirVersor) {_p+= (dirVersor*distance);}

private:
	GVector3 _p;
};

}		//EO namespace GMath
#endif		//GPOINT_H