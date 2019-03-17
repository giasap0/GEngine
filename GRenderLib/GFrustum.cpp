#include "GFrustum.h"

#include <DirectXMath.h>

void GFrustum::constructFrustum(Real screenDepth,GMatrix4x4 projectionMatrix,const GMatrix4x4& viewMatrix)
{
	//calculate the minimum Z distance in the frustum
	Real zMinimum = -1.0f* (projectionMatrix(4,3) / projectionMatrix(3,3));
	Real r = screenDepth/(screenDepth-zMinimum);
	projectionMatrix(3,3)=r;
	projectionMatrix(4,3)= -1.0f*(r*zMinimum);

	//create the frustum matrix from the view matrix and updated projection
	GMatrix4x4 frustumMatrix = viewMatrix*projectionMatrix;

	//near plane
	_planes[0].a() = frustumMatrix(1,4)+frustumMatrix(1,3);
	_planes[0].b() = frustumMatrix(2,4)+frustumMatrix(2,3);
	_planes[0].c() = frustumMatrix(3,4)+frustumMatrix(3,3);
	_planes[0].d() = frustumMatrix(4,4)+frustumMatrix(4,3);
	_planes[0].normalize();

	//far plane
	_planes[1].a() = frustumMatrix(1,4)-frustumMatrix(1,3);
	_planes[1].b() = frustumMatrix(2,4)-frustumMatrix(2,3);
	_planes[1].c() = frustumMatrix(3,4)-frustumMatrix(3,3);
	_planes[1].d() = frustumMatrix(4,4)-frustumMatrix(4,3);
	_planes[1].normalize();

	//left plane
	_planes[2].a() = frustumMatrix(1,4)+frustumMatrix(1,1);
	_planes[2].b() = frustumMatrix(2,4)+frustumMatrix(2,1);
	_planes[2].c() = frustumMatrix(3,4)+frustumMatrix(3,1);
	_planes[2].d() = frustumMatrix(4,4)+frustumMatrix(4,1);
	_planes[2].normalize();

	//right plane
	_planes[3].a() = frustumMatrix(1,4)-frustumMatrix(1,1);
	_planes[3].b() = frustumMatrix(2,4)-frustumMatrix(2,1);
	_planes[3].c() = frustumMatrix(3,4)-frustumMatrix(3,1);
	_planes[3].d() = frustumMatrix(4,4)-frustumMatrix(4,1);
	_planes[3].normalize();

	//top plane
	_planes[4].a() = frustumMatrix(1,4)-frustumMatrix(1,2);
	_planes[4].b() = frustumMatrix(2,4)-frustumMatrix(2,2);
	_planes[4].c() = frustumMatrix(3,4)-frustumMatrix(3,2);
	_planes[4].d() = frustumMatrix(4,4)-frustumMatrix(4,2);
	_planes[4].normalize();

	//bottom plane
	_planes[5].a() = frustumMatrix(1,4)+frustumMatrix(1,2);
	_planes[5].b() = frustumMatrix(2,4)+frustumMatrix(2,2);
	_planes[5].c() = frustumMatrix(3,4)+frustumMatrix(3,2);
	_planes[5].d() = frustumMatrix(4,4)+frustumMatrix(4,2);
	_planes[5].normalize();
}

bool GFrustum::checkPoint(const GPoint& point) const
{
	GVector4 v4( point.getPosition() );
	v4.w()=1.0f;
	GMath::Real d = 0.0f;
	for(int i=0; i<6;i++)
	{
		d = _planes[i].vector() * v4;
		if (d <0.0f)
			return false;
	}
	return true;
}

//CheckCube checks if any of the eight corner points of the cube are inside the viewing frustum
bool GFrustum::checkCube(const GPoint& center, Real radius) const
{
	for(int i=0; i<6;i++)
	{
		if ( ! checkPoint( GPoint(center.x()-radius, center.y()-radius, center.z()-radius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+radius, center.y()-radius, center.z()-radius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()-radius, center.y()+radius, center.z()-radius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()-radius, center.y()-radius, center.z()+radius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+radius, center.y()+radius, center.z()+radius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()-radius, center.y()+radius, center.z()+radius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+radius, center.y()-radius, center.z()+radius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+radius, center.y()+radius, center.z()-radius ) ) )
			continue;
		return false;
	}
	return true;
}

bool GFrustum::checkSphere(const GPoint& center, Real radius) const
{
	if(radius<0.0f)
		radius*=-1;
	GVector4 v4( center.getPosition() );
	v4.w()=1.0f;
	GMath::Real d = 0.0f;
	for(int i=0; i<6;i++)
	{
		d = _planes[i].vector() * v4;
		if(d<-radius)
			return false;
	}
	return true;
}

//works like cube but we have different radius
bool GFrustum::checkRectangle(const GPoint& center, Real xRadius, Real yRadius, Real zRadius) const
{
	for(int i=0; i<6;i++)
	{
		if ( ! checkPoint( GPoint(center.x()-xRadius, center.y()-yRadius, center.z()-zRadius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+xRadius, center.y()-yRadius, center.z()-zRadius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()-xRadius, center.y()+yRadius, center.z()-zRadius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()-xRadius, center.y()-yRadius, center.z()+zRadius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+xRadius, center.y()+yRadius, center.z()+zRadius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()-xRadius, center.y()+yRadius, center.z()+zRadius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+xRadius, center.y()-yRadius, center.z()+zRadius ) ) )
			continue;
		if ( ! checkPoint( GPoint(center.x()+xRadius, center.y()+yRadius, center.z()-zRadius ) ) )
			continue;
		return false;
	}
	return true;
}

/*
DBG_OUTPT("Frustum Planes: ");
	DBG_OUTPT("near normal = " << _planes[0].normalVector().toString().constData() );
	DBG_OUTPT("far normal = " << _planes[1].normalVector().toString().constData() );
	DBG_OUTPT("left normal = " << _planes[2].normalVector().toString().constData() );
	DBG_OUTPT("right normal = " << _planes[3].normalVector().toString().constData() );
	DBG_OUTPT("top normal = " << _planes[4].normalVector().toString().constData() );
	DBG_OUTPT("bottom normal = " << _planes[5].normalVector().toString().constData() );
*/