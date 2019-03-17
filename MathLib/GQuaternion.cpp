#include "GQuaternion.h"


using namespace GMath;

//q = [ cos(t/2) , V*sin(t/2) ]
GQuaternion::GQuaternion(Radians angle, const GVector3& rotationVersor)
{
	_w = GMath::cos(angle/2.0f);
	Real sint = GMath::sin(angle/2.0f);
	_v.x()=rotationVersor.x()*sint;
	_v.y()=rotationVersor.y()*sint;
	_v.z()=rotationVersor.z()*sint;
}
GQuaternion::GQuaternion(const GMatrix3x3& M)
{
	if( M.trace() >= .0f)
	{
		Real t = M.trace()+1.0f;
		Real s = GMath::reciprocalSqrt(t)*0.5f;
		_w = s*t;
		_v.x() = (M(3,2)-M(2,3) )*s;
		_v.y() = (M(1,3)-M(3,1) )*s;
		_v.z() = (M(2,1)-M(1,2) )*s;
	}else if(M(1,1) > M(2,2) && M(1,1)>M(3,3) )
	{
		Real t = M(1,1)-M(2,2)-M(3,3)+1.0f;
		Real s = reciprocalSqrt(t)*0.5f;
		_v.z() = s*t ;
		_v.y() = (M(2,1)+M(1,2) )*s;
		_v.x() = (M(1,3)+M(3,1) )*s;
		_w = (M(3,2)-M(2,3) )*s;
	}else if(M(2,2) > M(3,3))
	{
		Real t = -M(1,1)+M(2,2)-M(3,3)+1.0f;
		Real s = reciprocalSqrt(t)*0.5f;
		_v.y() = s * t; 
        _v.x() = ( M(2,1) + M(1,2) ) * s; 
        _w = ( M(1,3) - M(3,1) ) * s; 
        _v.z() = ( M(3,2) + M(2,3) ) * s;
	}else
	{
		Real t = -M(1,1)-M(2,2)+M(3,3)+1.0f;
		Real s = reciprocalSqrt(t)*0.5f;
		_v.z() = s * t; 
        _w = ( M(2,1) - M(1,2) ) * s; 
        _v.x() = ( M(1,3) + M(3,1) ) * s; 
        _v.y() = ( M(3,2) + M(2,3) ) * s;
	}
}

GQuaternion GQuaternion::operator *(const GQuaternion& other) const
{   //(w1, v1)*(w2,v2) = (w1w2 - v1v2, w1v2+ w2v1 + v1^v2)
    Real newW = (_w*other.w())-(_v*other.v());
	GVector3 newV = (_w*other.v()) +(other.w()*_v)+ (_v^other.v());
    return GQuaternion(newV,newW);
}
GQuaternion& GQuaternion::operator *=(const GQuaternion& other)
{
    Real newW = (_w*other.w())-(_v*other.v());
	GVector3 newV = (_w*other.v()) +(other.w()*_v)+ (_v^other.v());
    _w = newW;
    _v = newV;
    return *this;
}


GMatrix3x3 GQuaternion::rotationMatrix() const
{
	Real Nq = normSquare();
	Real s;
	if( areEqual(Nq,0.0f) )
		s=0.0f;
	else
		s=2.0f/Nq;
	Real X = _v.x()*s,			Y=_v.y()*s,				Z = _v.z()*s;
	Real wX = _w*X,				wY = _w*Y,				wZ = _w*Z;
	Real xX = _v.x()*X,			xY = _v.x()*Y,			xZ = _v.x()*Z;
	Real yY = _v.y()*Y,			yZ = _v.y()*Z,			zZ = _v.z()*Z;
	GMatrix3x3 M;
	M(1,1)=1.0f-(yY+zZ);		M(1,2)=xY-wZ;			M(1,3)=xZ+wY;
	M(2,1)=xY+wZ;				M(2,2)=1.0f-(xX+zZ);	M(2,3)=yZ-wX;
	M(3,1)=xZ-wY;				M(3,2)= yZ+wX;			M(3,3)=1.0f-(xX+yY);
	return M;
}

void GQuaternion::axisAngleEquivalent(Radians& modificableAngle, GVector3& modificableVect) const
{
	Real k = _v.norm();
    if(areEqual(k,0.0f))
    {
        modificableAngle = 0.0;
		modificableVect = GVector3();
        modificableVect[0]=1.0;
        return;
    }
	else if(areEqual(k,1.0f))
	{
		modificableAngle = 2.0f * acos(_w);
		modificableVect[0] = _v.at(0);
		modificableVect[1] = _v.at(1);
		modificableVect[2] = _v.at(2);
		return;
	}
	else
	{
		modificableAngle = 2.0f * acos(_w);
		modificableVect[0] = _v.at(0)/k;
		modificableVect[1] = _v.at(1)/k;
		modificableVect[2] = _v.at(2)/k;
		return;
	}
}

												// FUNCTIONS

void GMath::rotateVector(GVector3& v, const GQuaternion& q)
{
	GQuaternion V(v,0.0f);
	GQuaternion R = q*V*q.conjugate();
	v= R.v();
}
void GMath::SLerp(const GQuaternion& p, const GQuaternion& q, Real beta, _Out_ GQuaternion& SL)
{
	beta = ( beta > 1.0f) ? 1.0f : (( beta < 0.0f) ? 0.0f :  beta);  // Clamp value to 0 to 1
	Radians teta = arcCos( (p.x()*q.x()) + (p.y()*q.y()) + (p.z()*q.z()) + (p.w()*q.w()) );
	Real sinteta = sin(teta);
	Real wp = sin( teta*(1-beta) )/sinteta;
	Real wq = sin(teta*beta)/ sinteta;
	SL = (p*wp) + (q*wq);
}
