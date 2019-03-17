#pragma once
#ifndef GEQUATION_SOLVER_H
#define GEQUATION_SOLVER_H

#include "GComplex.h"
#include <GVector.h>

namespace GMath
{

class GEquationSolver
{
public:
	//solve ax^2 +bx + c=0;
    static GVector<GComplex> solveQuadratic(Real a, Real b, Real c);
	//solve ax^2 +bx + c=0;
    static GVector<GComplex> solveQuadratic(GComplex a, GComplex b, GComplex c);
	//solve ax^3 + bx^2 + cx + d=0
    static GVector<GComplex> solveCubic(Real a, Real b, Real c, Real d);
private:
	static GComplex scegliRealeOprimo(const GComplex* v);
	//DO NOT DEFINE THEM
	GEquationSolver();
	GEquationSolver(const GEquationSolver& eq);
	GEquationSolver& operator=(const GEquationSolver& eq);
};
}//EO namespace GMath

#endif // GEQUATION_SOLVER_H