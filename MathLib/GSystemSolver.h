#pragma once
#ifndef GSYSTEMSOLVER_H
#define GSYSTEMSOLVER_H

#include "GVector4.h"
#include "GVector2.h"

namespace GMath
{
class GSystemSolver
{
public:
	//equations like ax+by+d=0
	static GVector2 solve2D_linear(const GVector3& firstEq, const GVector3& secondEq);
	//equations like ax+by+cz+d=0
	static GVector3 solve3D_linear(const GVector4& firstEq, const GVector4& secondEq, const GVector4& thirdEq);

private:
	//DO NOT DEFINE THEM
	GSystemSolver(void);
	GSystemSolver(const GSystemSolver& sys);
	GSystemSolver& operator=(const GSystemSolver& sys);
};
}//EO namespace GMath

#endif	//EO GSYSTEMSOLVER
