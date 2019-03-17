#include "GComplex.h"

using namespace GMath;

GComplex::GComplex(const Real& length, const Radians& alfa)
{
	//*****************************************
    //**  tg(alfa)=img/Real
    //**  length = sqrt(img^2+Real^2)
    //**  img= Real*tg(alfa)
    //**  Real^2 = lenght^2-img^2
    //**  Real^2 = lenght^2- Real^2*tg^2(alfa)
    //**  Real^2 = lenght^2/(1+tg^2(alfa))
    //******************************************
	Real tgQuadro = pow2( tan(alfa) );
	Real rquadro = pow2(length)/(1.0f+tgQuadro);
	_r = sqrt(rquadro);
	Real tanAlfa= tan(alfa);
	_im=_r*tan(alfa);			//in pi/2 and -pi/2 tan go to inf
	if( areEqual(alfa.toReal() , -Constants::pi_half ) )
		_im*=-1;
}
Radians GComplex::angleRadians() const
{
	if(_r>0.0)
	{
		return Radians(atan(_im/_r));
	}
	else if(_r<0.0)
	{
		return Radians(atan(_im/_r)+Constants::pi);
	}
	else
		return Radians(Constants::pi_half);
}
Degree GComplex::angleDegree() const
{
	return Degree(angleRadians() );
}
GString GComplex::toString() const
{
	if(_im>=0.0f)
		return GString::number(_r) + " + i " + GString::number(_im);
	else
		return GString::number(_r) + " - i " + GString::number(-_im);
}
GString GComplex::toStringEuler(bool angleInRadians) const
{
	GString str = GString::number( norm() ) + " *e^(i ";
	if(angleInRadians)
	{   //fi *pi = angle
		Real fi = angleRadians().toReal()/Constants::pi;
		str += GString::number(fi) + " *pi)";
	}
	else
	{
		str += GString::number( angleDegree().toReal() ) + "° )";
	}
	return str;
}
bool GComplex::fromString(const GString & string, const GString& format)
{
	if(format=="a + i b")
	{
		 if(!string.contains("+ i"))
            return false;
        GStringList list = string.split("+ i");
        if(list.isEmpty())
            return false;
		_r = list.at(0).toFloat();
        _im = list.at(1).toFloat();
        return true;
	}
	if(format == "a+ib" || format == "a +i b"|| format == "a+i b"|| format == "a +ib")
    {
        if(!string.contains("+i"))
            return false;
        GStringList list = string.split("+i");
        if(list.isEmpty())
            return false;
        _r = list.at(0).toFloat();
        _im = list.at(1).toFloat();
        return true;
    }
	if(format == "a *e^(i b*pi)" || format == "a *e^(i b *pi)" || format == "a *e^(ib*pi)"
            || format == "a*e^(ib*pi)"|| format == "a*e^(i b*pi)"|| format == "a *e^(i b *pi)")
    {
        if(!string.contains("*e^(i"))
            return false;
        GStringList list = string.split("*e^(i");
        if(list.isEmpty())
            return false;
        if(!list.at(1).contains("*pi"))
            return false;
        GStringList list2 = list.at(1).split("*pi");
        if(list.isEmpty())
            return false;
		setValues( list.at(0).toFloat(),Radians(list2.at(0).toFloat() ) );
        return true;
    }
	if(format == "a *e^(i b°)" || format == "a *e^(i b °)" || format == "a *e^(ib°)"
            || format == "a*e^(ib°)"|| format == "a*e^(i b°)")
    {
        if(!string.contains("*e^(i"))
            return false;
        GStringList list = string.split("*e^(i");
        if(list.isEmpty())
            return false;
        if(!list.at(1).contains("°"))
            return false;
        GStringList list2 = list.at(1).split("°");
        if(list.isEmpty())
            return false;
		setValues(list.at(0).toFloat() , Degree (list2.at(0).toFloat()) );
        return true;
    }
    return false;
}

void GComplex::setValues(const Real& length, const Radians& alfa)
{
	Real tgQuadro = pow2(tan(alfa));
	Real rquadro = pow2(length)/(1.0f+tgQuadro);
	_r = sqrt(rquadro);
	_im=_r*tan(alfa);
}

GComplex GComplex::reciprocal() const
{
	Real denom = ( pow2(_r)+pow2(_im) );
    return GComplex(_r/denom , (-1.0f)*_im/denom );
}
GComplex GComplex::exponentiation(const Real& exponent) const
{
	//c^n = norm^n * e^(i n*fi)
    if(areEqual(_im,0.0f))
        return GComplex(pow(_r,exponent));
    Real normTo_n = pow(norm(),exponent);
	Radians nuovoAngolo = angleRadians()*exponent;
    return GComplex(normTo_n,nuovoAngolo);
}
GComplex GComplex::operator *(const GComplex& c) const
{
    Real newReal= (_r*c.real())-(_im*c.img());
    Real newImg = (_im*c.real())+(_r*c.img());
    GComplex result(newReal,newImg);
    return result;
}
GComplex& GComplex::operator *=(const GComplex& c)
{
	
    Real newReal =(_r*c.real())-(_im*c.img());
    Real newImg = (_im*c.real())+(_r*c.img());
	_r=newReal;
	_im=newImg;
    return *this;
}
bool GComplex::operator ==(const GComplex& c) const
{
    if(areEqual(_r,c.real()) && areEqual(_im,c.img()))
        return true;
    return false;
}
bool GComplex::operator ==(const Real& d) const
{
    if(areEqual(_im,0.0f) && areEqual(_r,d))
        return true;
    return false;
}
bool GComplex::operator !=(const GComplex& c) const
{
    //if(r!=c.real() || im!=c.img())
    if(!areEqual(_r,c.real()) || !areEqual(_im,c.img()))
        return true;
    return false;
}
bool GComplex::operator !=(const Real& d) const
{
    if(areEqual(_im,0.0f) && areEqual(_r,d))
        return false;
    return true;
}
//using normsquare is faster then norm, in this contest don't make sense to sqrRoot both members
bool GComplex::operator >(const GComplex& c) const
{
	if( normSquare() > c.normSquare() )
        return true;
    return false;
}
bool GComplex::operator >=(const GComplex& c) const
{
    if( normSquare() >= c.normSquare() )
        return true;
    return false;
}
bool GComplex::operator <(const GComplex& c) const
{
    if( normSquare() < c.normSquare() )
        return true;
    return false;
}
bool GComplex::operator <=(const GComplex& c) const
{
    if( normSquare() <= c.normSquare() )
        return true;
    return false;
}

                                    //      FRIENDS     //

GComplex GMath::pow(const GComplex& c, const Real& exponent)
{
    //c^n = norm^n * e^(i n*fi)
    if(areEqual(c.img(),0.0f))
		return GMath::pow(c.real(),exponent);
	Real normTo_n = GMath::pow(c.normSquare(),exponent/2.0f);
	Radians vecchioAngolo = c.angleRadians();
	Radians nuovoAngolo = c.angleRadians()*exponent;
    return GComplex(normTo_n,nuovoAngolo);
}

GComplex GMath::e_powToComplex(const GComplex& exponent)
{
	//e^c = (e^Real)*[cos(img)+i*sin(img)]
	Real k = pow(Constants::euler,exponent._r);
	Real newReal = k* cos(Radians(exponent._im));
    Real newImg =  k* sin(Radians(exponent._im));
    return GComplex(newReal, newImg);
}
GComplex GMath::ln(const GComplex& c)
{
    Real newReal = ln(c.norm());
	Real newImg = c.angleRadians().toReal();
    return GComplex(newReal, newImg);
}
GComplex GMath::sqRoot(const GComplex& c)
{
    //if(c.img() ==0 && c.real()>=0)
    if(areEqual(c.img(),0.0f) && c.real()>0.0)
        return GComplex(GMath::sqrt(c.real()),0.0f);
    //if(c.img()==0 && c.real()<=0)
    if(areEqual(c.img(),0.0f) && c.real()<=0.0)
        return GComplex(0.0,GMath::sqrt(-c.real()));
    //c^n = norm^n * e^(i n*fi)
	Real normTo_n = GMath::pow(  c.normSquare(), 0.25f );	//==norm^0.5
	Radians nuovoAngolo = c.angleRadians()*0.5f;
    return GComplex(normTo_n,nuovoAngolo);
}
GComplex GMath::sqRoot(Real d)
{
    if(d>=0.0)
        return GComplex (GMath::sqrt(d),0.0);
    else
        return GComplex (0,GMath::sqrt(-d));      //es: sqrt(-2) = 0+i sqrt(2)
}
GVector<GComplex> GMath::nRoot(const GComplex& c, int n)
{
	if(n<=0)
    {
        GVector<GComplex> emptyVect;
        return emptyVect;
    }
	if(n==1)
	{
		GVector<GComplex> res(1);
		res[0] = c;
		return res;
	}

    // norm^(1/root) * [ cos ((fi+2kp)/root) +i sin ((fi+2kp)/root) ]
	Real rootNorm = pow(c.normSquare(),0.5f/castReal(n));
	GVector<Radians> angles(n);
	Radians fi = c.angleRadians();
    for(int k=0; k<n; k++)
		angles[k] = (fi+( k*Constants::pi_double))/castReal(n);
    GVector<GComplex> solutions(n);
    for(uint k=0; k<solutions.size(); k++)
		solutions[k]= (GComplex ( cos(angles.at(k)) , sin(angles.at(k)) ) )*rootNorm;
    return solutions;
}
GComplex GMath::operator+(const Real& a, const GComplex& b)
{
	return b+a;
}
GComplex  GMath::operator -(const Real& a, const GComplex& b)
{
    return GComplex(a-b._r,(b._im*(-1)));
}
GComplex GMath::operator *(const Real& a, const GComplex& b)
{
    return b*a;
}
GComplex GMath::operator /(const Real& a, const GComplex& b)
{
	GComplex inv = pow(b,-1);
    return inv*a;
}
bool GMath::operator ==(const Real& a, const GComplex& b)
{
    //if(b.im==0.0 && a==b.r)
    if(areEqual(b.img(),0.0f) && areEqual(a,b.real()))
        return true;
    return false;
}
bool GMath::operator !=(const Real&a, const GComplex& b)
{
    if(a == b)
        return false;
    return true;
}
