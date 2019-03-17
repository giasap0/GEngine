#pragma once
#ifndef GSCENE_H
#define GSCENE_H

#include "GRenderIncludes.h"
#include "Shaders\GColors.h"
#include <GMath.h>

//--------------------------------------------------------------------------------------
//Class GScene: maintaining information about all the models in the scene
//--------------------------------------------------------------------------------------
class GScene
{
	struct ModelInfoType
	{
		GMath::GVector3 position;
		GMath::GMatrix3x3 rotation;

		bool operator==(const ModelInfoType& other) const
		{
			return (position == other.position) && (rotation== other.rotation);
		}
	};

public:
	GScene(void)										{}
	~GScene(void)										{}

	void shutDown();
	bool initialize(int numElements);

	inline GMath::uint32 getModelCount() const			{return _modelInfoList.size();}

	void getData(GMath::uint32 elementId, _Out_ GMath::GVector3& position, _Out_ GMath::GMatrix3x3& rotation);
	
	inline void getAirplane(_Out_ GMath::GVector3& position, _Out_ GMath::GMatrix3x3& rotation)
	{
		getData(0, position, rotation);
	}

	inline void setAirplane(const GMath::GVector3& position, const GMath::GMatrix3x3& rotation)
	{
		_modelInfoList[0].position = position;
		_modelInfoList[0].rotation = rotation;
	}

	inline void setAirplane(const GMath::GMatrix4x4& planeStatus)
	{
		_modelInfoList[0].position = GMath::GVector3(planeStatus.translationVector());
		_modelInfoList[0].rotation = planeStatus.subMatrix();
	}

	inline void getGround(_Out_ GMath::GVector3& position)
	{
		position = _modelInfoList[1].position;
	}
	inline void setGroundPosition(const GMath::GVector3& position)
	{
		_modelInfoList[1].position = position;
	}
	inline void moveGround(const GMath::GVector3& move)
	{
		_modelInfoList[1].position += move;
	}
	inline void moveGround(GMath::Real x, GMath::Real y, GMath::Real z)
	{
		_modelInfoList[1].position.x() += x;
		_modelInfoList[1].position.x() += y;
		_modelInfoList[1].position.x() += z;
	}

private:
	GVector<ModelInfoType> _modelInfoList;

	//prevent copy
	GScene(const GScene&);
	GScene& operator=(const GScene&);
};

#endif	//GSCENE_H