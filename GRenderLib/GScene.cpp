#include "GScene.h"

using GMath::uint32;

bool GScene::initialize(int numElements)
{
	_modelInfoList.reserve(numElements);

	GMath::GRandom randomizer;
	GMath::Real red, green, blue;
	GColor color;
	int isNegative;

	for(int i=0; i<numElements; i++)
	{
		ModelInfoType element;
		_modelInfoList.append(element);

		red = randomizer.random_zeroOne();
		green = randomizer.random_zeroOne();
		blue = randomizer.random_zeroOne();
		_modelInfoList[i].color.setValues(red,green,blue,1);

		_modelInfoList[i].position.x() = GMath::castReal<uint32>(randomizer.random(0,8));
		_modelInfoList[i].position.y() = GMath::castReal<uint32>(randomizer.random(0,3));
		_modelInfoList[i].position.z() = GMath::castReal<uint32>(randomizer.random(0,50));

		isNegative= randomizer.random(1);
		if(isNegative==1)
			_modelInfoList[i].position.x() *=-1.0f;
		isNegative = randomizer.random(1);
		if(isNegative==1)
			_modelInfoList[i].position.y() *=-1.0f;
		//isNegative = randomizer.random(1);
		//if(isNegative==1)
		//	_modelInfoList[i].position.z() *=-1.0f;
	}
	if( _modelInfoList.size() != numElements)
	{
		DBG_OUTPT("GScene::initialize - something went wrong. Cannot create " << numElements << " elements");
		return false;
	}
	DBG_OUTPT("GScene initialized. Sucesfully Created " << numElements << " elements");
	return true;
}

void GScene::shutDown()
{
	_modelInfoList.clear();
}

void GScene::getData(GMath::uint32 elementId, _Out_ GMath::GPoint& position, _Out_ GColor& color)
{	
	position = _modelInfoList[elementId].position;
	color = _modelInfoList[elementId].color;
	return;
}



/*
DBG_OUTPT("GScene::init - model id = " << i);
		DBG_OUTPT("position = " << _modelInfoList[i].position.toString().constData() );
		color = _modelInfoList[i].color;
		DBG_OUTPT("color = (" << color.red << ", " << color.green << ", "<< color.blue << ", " << color.alpha << ")");
*/