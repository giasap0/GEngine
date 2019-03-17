#pragma once
#ifndef GFRUSTUM_H
#define GFRUSTUM_H

/*************************************************************************************************************************
**	Each frame the ConstructFrustum function is called after the camera has first been rendered.						**
**	The ConstructFrustum function uses the private _planes to calculate and store the six planes of the view frustum	**
**	based on the updated viewing location.																				**
*************************************************************************************************************************/

#include "GRenderIncludes.h"
#include <GMath.h>
using namespace GMath;

//represent the frustum of view. Use for frustum culling
class GFrustum
{
public:
	GFrustum(void)									{}
	virtual ~GFrustum(void)							{}

	void shutdown()									{}

	void constructFrustum(Real screenDepth, GMatrix4x4 projectionMatrix,const GMatrix4x4& viewMatrix);

	bool checkPoint(const GPoint& point) const;
	inline bool checkPoint(Real x, Real y, Real z) const				{return checkPoint( GPoint(x,y,z) );}
	//radius == side/2
	bool checkCube(const GPoint& center, Real radius) const;
	bool checkSphere(const GPoint& center, Real radius) const;
	//xRadius, yRadius and zRadius are x-y-z sides/2
	bool checkRectangle(const GPoint& center, Real xRadius, Real yRadius, Real zRadius) const;

private:
	GPlane	_planes[6];

	//prevent copy
	GFrustum(const GFrustum&);
	GFrustum& operator= (const GFrustum&);
};



#endif	//GFRUSTUM_H