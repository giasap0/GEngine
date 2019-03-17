#include "GScene.h"

using GMath::uint32;
using namespace GMath;

bool GScene::initialize(int numElements)
{
	_modelInfoList.reserve(numElements);

	GMath::GRandom randomizer;

	ModelInfoType element_Airplane;
	_modelInfoList.append(element_Airplane);

	_modelInfoList[0].position.x() = 0.0f;
	_modelInfoList[0].position.y() = 0.0f;
	_modelInfoList[0].position.z() = 0.0f;

	ModelInfoType element_Ground;
	_modelInfoList.append(element_Ground);

	_modelInfoList[1].position.x() = 0.0f;
	_modelInfoList[1].position.y() = 0.00f;
	_modelInfoList[1].position.z() = -0.5f;

	DBG_OUTPT("GScene initialized. Sucesfully Created " << _modelInfoList.size() << " elements");
	return true;
}

void GScene::shutDown()
{
	_modelInfoList.clear();
}

void GScene::getData(GMath::uint32 elementId, _Out_ GMath::GVector3& position, _Out_ GMath::GMatrix3x3& rotation)
{	
	position = _modelInfoList[elementId].position;
	rotation = _modelInfoList[elementId].rotation;
	return;
}

/*
DBG_OUTPT("GScene::init - model id = " << i);
		DBG_OUTPT("position = " << _modelInfoList[i].position.toString().constData() );
		color = _modelInfoList[i].color;
		DBG_OUTPT("color = (" << color.red << ", " << color.green << ", "<< color.blue << ", " << color.alpha << ")");
*/