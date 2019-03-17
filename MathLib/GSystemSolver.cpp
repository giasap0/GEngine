#include "GSystemSolver.h"
#include "GMatrix4x4.h"
#include <GEngineCore.h>
#include <Windows.h>		//needed for debug

using namespace GMath;

GVector2 GSystemSolver::solve2D_linear(const GVector3& firstEq, const GVector3& secondEq)
{
	Real D = ( firstEq.x() * secondEq.y() ) - ( firstEq.y() * secondEq.x() );
	if( areEqual(D, .0f, Constants::realMin) )
	{
		DBG_OUTPT("GSystemSolver::solve2D_linear - determinant==0 returning empty");
		return GVector2();
	}
	Real N = ( -firstEq.z() * secondEq.y() ) - ( firstEq.y() * (-secondEq.z()) );
	GVector2 solution;
	solution.x() = N/D;
	N = ( firstEq.x() * (-secondEq.z()) ) - ( -firstEq.z() * secondEq.x() );
	solution.y() = N/D;
	return solution;
}

GVector3 GSystemSolver::solve3D_linear(const GVector4& firstEq, const GVector4& secondEq, const GVector4& thirdEq)
{
	GMatrix3x3 M;
	M.replaceRow(firstEq.get3DVect(),0);
	M.replaceRow(secondEq.get3DVect(),1);
	M.replaceRow(thirdEq.get3DVect(),2);
	Real D = M.determinant();
	if( areEqual(D, .0f, Constants::realMin) )
	{
		DBG_OUTPT("GSystemSolver::solve3D_linear - determinant==0 returning empty");
		return GVector3();
	}
	GVector3 b( -firstEq.w(), -secondEq.w(),-thirdEq.w() );
	GVector3 solution;
	GMatrix3x3 N = M;
	N.replaceColumn(b,0);
	solution.x() = N.determinant() /D;
	N = M;
	N.replaceColumn(b,1);
	solution.y() = N.determinant() /D;
	N = M;
	N.replaceColumn(b,2);
	solution.z() = N.determinant() /D;
	return solution;
}
