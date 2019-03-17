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
		GColor color;
		GMath::GPoint position;

		inline ModelInfoType& operator=(const ModelInfoType& t2)			{color = t2.color;	position = t2.position;	return *this;}
		inline bool operator==(const GScene::ModelInfoType t2) const
		{
			if( color==t2.color && position==t2.position)
				return true;
			return false;
		}
	};

public:
	GScene(void)										{}
	~GScene(void)										{}

	void shutDown();
	bool initialize(int numElements);

	inline GMath::uint32 getModelCount() const			{return _modelInfoList.size();}

	void getData(GMath::uint32 elementId, _Out_ GMath::GPoint& position, _Out_ GColor& color);

private:
	GVector<ModelInfoType> _modelInfoList;

	//prevent copy
	GScene(const GScene&);
	GScene& operator=(const GScene&);
};


#endif	//GSCENE_H