#include "GRay.h"
#include "GSystemSolver.h"
#include <GEngineCore.h>
#include <Windows.h>			//needed for debug
using namespace GMath;

//this function choose a parameter to calculate a point that belongs to both planes
//-1==null plane, 0==whatever, 1==x, 2==y, 3==z
Int8 chooseParameter(const GPlane& p)
{
	GVector4 v1 = p.vector(); 
	if(v1.isNull() || p.normalVector().isNull() )
		return -1;
	if( areEqual(v1.y(),.0f) && areEqual(v1.z(),.0f) )
		return 1;
	if( areEqual(v1.x(),.0f) && areEqual(v1.z(),.0f) )
		return 2;
	if( areEqual(v1.x(),.0f) && areEqual(v1.y(),.0f) )
		return 3;
	return 0;
}

//find one point that belongs to both planes
GPoint findAPoint(const GPlane& p1, const GPlane& p2)
{
	Int8 param1 = chooseParameter(p1);
	Int8 param2 = chooseParameter(p2);
	GPoint P;
	if(param1 == 1)
	{
		P.x()=p1.d()/p1.a();
		if(param2==2)
		{
			P.y() = p2.d()/p2.b();
			P.z()=.0f;
		}
		else if(param2==3)
		{
			P.y()=.0f;
			P.z() = p2.d()/p2.c();
		}
		else if(param2==0)
		{
			P.z()=0;
			P.y() = ( p2.d() - (p2.a()*P.x()) )/p2.b();
		}
	}//EO if(param1 == 1)
	else if(param1 == 2)
	{
		P.y()=p1.d()/p1.b();
		if(param2==1)
		{
			P.x() = p2.d()/p2.a();
			P.z()=.0f;
		}
		else if(param2==3)
		{
			P.z() = p2.d()/p2.c();
			P.x()=.0f;
		}
		else if(param2==0)
		{
			P.z()=0;
			P.x() = ( p2.d() - (p2.b()*P.y()) )/p2.a();
		}

	}//EO if(param1 == 2)
	else if(param1 == 3)
	{
		P.z() = p1.d()/p1.c();
		if(param2==1)
		{
			P.x() = p2.d()/p2.a();
			P.y()=.0f;
		}
		else if(param2==2)
		{
			P.y() = p2.d()/p2.b();
			P.x()=.0f;
		}
		else if(param2==0)
		{
			P.y()=0;
			P.x() = ( p2.d() - (p2.c()*P.z()) )/p2.a();
		}
	}//EO if(param1 == 3)
	else if( param1== 0)
	{
		if(param2==1)
		{
			P.x() = p2.d()/p2.a();
			P.z()=.0f;
			P.y() = ( p1.d() - (p1.a()*P.x()) )/p1.b();
		}
		else if(param2==2)
		{
			P.y() = p2.d()/p2.b();
			P.z()=0;
			P.x() = ( p1.d() - (p1.b()*P.y()) )/p1.a();
		}
		else if(param2==3)
		{
			P.z() = p2.d()/p2.c();
			P.y()=0;
			P.x() = ( p1.d() - (p1.c()*P.z()) )/p1.a();
		}
		else if(param2==0)
		{
			P.z()=0;
			if(p1.a()!=0)
			{
				Real a21 = p2.a()/p1.a();
				P.y()= ( p2.d() - (a21*p1.d()) ) / ( p2.b()- (a21*p1.b() ) );
				P.x() = (p1.d() - (p1.b()*P.y()) )/p1.a();
			}
			else if(p1.b()!=0)
			{
				Real b21 = p2.b() / p1.b();
				P.x() = (p2.d() - (b21*p1.d())) / (p2.a() - (b21*p1.a()) );
				P.y() = (p1.d() -(p1.a()*P.x()) )/p1.b();
			}
		}
	}//EO if(param1 == 0)
	return P;
}


GRay::GRay(const GPlane& p1, const GPlane& p2)
{
	//dir is normal to both plane's normals
	_dir = p1.normalVector() ^ p2.normalVector();
	if( _dir.isNull() )
	{
		DBG_OUTPT("GRay constructor - 2 parallel planes");
		return;
	}
	_dir = _dir.normalized();
	_p = findAPoint(p1, p2);
}

GRay::GRay(const GPoint& A, const GPoint& B)
{
	_p=A;
	_dir = B-A;
	_dir = _dir.normalized();
}

bool GRay::belongToRay(const GPoint& P) const
{
	if(P==_p)
		return true;
	GRay r(_p,P);
	if( this->isParallelTo(r) )
		return true;
	return false;
}

GPoint GRay::intersect(const GRay& r) const
{
	if( this->isParallelTo(r) )
	{
		if( belongToRay(r._p) )	//are coincident
			return _p;
		else
		{
			DBG_OUTPT("GRay::intersect - rays are parallel and not coincident");
			return GPoint();
		}
	}
	//to intersect have to be complanar
	GPlane plan(_p,_dir,r._dir);
	if( plan.belongToPlain(r._p) )
	{
		DBG_OUTPT("GRay::intersect - rays are not complanar");
		return GPoint();
	}
	//i get planes _|_ to plan AND parallel to rays
	GPlane pr( _dir^plan.normalVector(), _p);
	GPlane ps( r._dir^plan.normalVector(), r._p);
	//point is intersection of this 3 planes
	GPoint P;
	P.setPosition( GSystemSolver::solve3D_linear(plan.vector(), pr.vector(), ps.vector() ) );
	return P;
}

GPoint GRay::intersect(const GPlane& p) const
{
	GVector3 n = p.normalVector();
	if ( n.isNormalTo( _dir ) )
	{
		DBG_OUTPT("GRay::intersect - ray is parallel to Plane");
		return GPoint();
	}
	GVector3 tau, mu;
	tau = _dir^n;
	mu = tau^_dir;
	GPlane p1(tau, _p);
	GPlane p2(mu, _p);
	GPoint P;
	P.setPosition( GSystemSolver::solve3D_linear(p.vector(), p1.vector(), p2.vector() ) );
	return P;
}


Real GRay::distanceToPoint(const GPoint& point) const
{
	GPlane normPlane(_dir,point);						//plane normal to this ray and pass in point
	Real d = normPlane.distanceToPointRelative(_p);		//get distance from P0 to plane
	GPoint A=_p;
	A.moveThroughVersor(d,_dir);						//move P0 of distance to find point of intersection with plane
														//legit only because ray is normal to plane

	return (A-point).norm();							//return distance between point plane/ray and input point
}
