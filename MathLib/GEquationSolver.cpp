#include "GEquationSolver.h"
using namespace GMath;

GVector<GComplex> GEquationSolver::solveQuadratic(Real a, Real b, Real c)
{//ax^2 +bx + c=0;
	if(areEqual(a,0.0f) && areEqual(b,0.0f))
        return GVector<GComplex>();
    if(areEqual(a,0.0f) && !areEqual(b,0.0f) )
    {
        GVector<GComplex> x(1);
        x[0]= -c/b;
        return x;
    }
    if(areEqual(b,0.0f)) //ax^2=-c -> x^2=-c/a
        return nRoot(GComplex(-c/a,0.0),2);

    GComplex sq = sqRoot( pow2(b)-(4*a*c) )/(2.0f*a);
    Real k = b*(-1)/(2.0f*a);
    GVector<GComplex> solutions(2);
    solutions[0]=k+sq;
    solutions[1]=k-sq;
    return solutions;
}

GVector<GComplex> GEquationSolver::solveQuadratic(GComplex a, GComplex b, GComplex c)
{//ax^2 + bx + c=0
	if(a==0.0 && b==0.0)
        return GVector<GComplex>();
    if(a==0.0 && b!=0.0)
    {
        GVector<GComplex> x(1);
        x[0]= -c/b;
        return x;
    }
    if(b==0)   //ax^2=-c -> x^2=-c/a
        return nRoot((-c/a),2);

    GVector<GComplex> sq = nRoot(pow(b,2.0f)-(4*a*c),2);
    sq[0] = sq[0]/(a*2.0);
    sq[1] = sq[1]/(a*2.0);
    GComplex k = b*(-1)/(2.0*a);
    GVector<GComplex> solutions(2);
    solutions[0]=k+sq.at(0);
    solutions[1]=k+sq.at(1);
    return solutions;
}

GVector<GComplex> GEquationSolver::solveCubic(Real a, Real b, Real c, Real d)
{
	//ax^3 + bx^2 + cx + d=0
    if(areEqual(a,0.0f))
        return solveQuadratic(b,c,d);
    GVector<GComplex> solutions(3);
    if(areEqual(b,0.0f) && areEqual(c,0.0f) && areEqual(d,0.0f))
    {
        solutions[0] = solutions[1] = solutions[2] = 0.0;
        return solutions;
    }
    else if(areEqual(b,0.0f) && areEqual(c,0.0f) && !areEqual(d,0.0f))
	{
        return nRoot(GComplex(-d,0.0),3);
	}
    if(areEqual(d,0.0f))
    {
        //ax^3 + bx^2 + cx =0 --> ax^2+bx +c=0 + soluzione x=0
        // se b==0 --> ax^3 + cx=0 --> ax^2+c=0 + soluzione x=0
        solutions= solveQuadratic(a,b,c);
        solutions.append(GComplex(0,0));
        return solutions;
    }

    Real k = -b/(3*a);
    Real h = (2.0f*pow_int(b,3)) - (9*a*b*c)+(27*a*a*d);
    Real sq = pow2(h) -(4*pow_int(((b*b)-(3*a*c)),3));
    GComplex Q = sqRoot(sq);
    Real g = (b*b)-(3*a*c);
    if(Q==0.0 && !areEqual(g,0.0f))
    {
        solutions[0]= solutions[1] = ((b*c)-(9*a*d))/(2*((3*a*c)-pow2(b)));
        solutions[2] = ((9*a*a*d)-(4*a*b*c)+pow_int(b,3))/(a*((3*a*c)-pow2(b)));
        return solutions;
    }
    if(Q==0.0 && areEqual(g,0.0f))
    {
        solutions[0] = solutions[1] = solutions[2] = k;
        return solutions;
    }
    //GComplex C = pow(0.5*(Q+(2*pow(b,3.0))-(9*a*b*c)+(27*a*a*d)),1/3.0);
    GComplex C2 = 0.5*(Q+(2*pow_int(b,3))-(9*a*b*c)+(27*a*a*d));
    GVector< GComplex> v = nRoot(C2,3);
	GComplex C = scegliRealeOprimo( v.data() );

    if(Q!=0 && areEqual(g,0.0f) && C2==0)    //il segno di Q va scelto in modo che C!=0
    {
        Q = -Q;
        C2 = 0.5*(Q+(2*pow_int(b,3))-(9*a*b*c)+(27*a*a*d));
        v = nRoot(C2,3);
		C = scegliRealeOprimo( v.data() );
    }
    GComplex G = g/(3*a*C);

    Real sqrt3 = sqrt(3.0f);
    solutions[0] = k - (C/(3*a)) -G;
    solutions[1] = k+ ((GComplex(1,sqrt3)/(6*a))*C) +(GComplex(1,(-1.0f)*sqrt3)*G/2);
    solutions[2] = k+ ((GComplex(1,(-1.0f)*sqrt3)/(6*a))*C) +(GComplex(1,sqrt3)*G/2);
    return solutions;
}
GComplex GEquationSolver::scegliRealeOprimo(const GComplex* v)
{
    for(int k=0; k<3; k++)      //se esiste prendo la radice reale diversa da 0
    {
        if( v[k].isReal() && v[k]!=0.0)
            return v[k];
    }

    for(int k=0; k<3; k++)//se non c'erano radici reali prendo la prima (fase= fi/n)
    {
        if (v[k]!=0.0) //oppure prendo il primo non nullo
            return v[k];
    }
    return GComplex(0,0);
}

/*
GVector<GComplex> equationSolver::solveCubic(Real a, Real b, Real c, Real d) const
{
    
}



*/