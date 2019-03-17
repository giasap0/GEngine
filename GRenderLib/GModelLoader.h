#pragma once
#ifndef GMODEL_LOADER_H
#define GMODEL_LOADER_H

#include "GRenderIncludes.h"

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using GMath::GVector3;
using GMath::Int32;


namespace ModelsStructs
{
	struct BasicModelData
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 text;
	};
	struct NormalMaModelData
	{
		XMFLOAT3 pos;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

}
//--------------------------------------------------------------------------------------
//Class : GModel : allow to load models from file into structs
//--------------------------------------------------------------------------------------
class GModelLoader
{
public:
	GModelLoader(void)						{}
	virtual ~GModelLoader(void)				{}

	//load model without skeleton, without tangent & binormal
	static bool loadStaticModel(const char* modelFileName, Int32& vertexCount, Int32&indexCount, ModelsStructs::BasicModelData** modelPtr);

	static bool loadNormalMapModel(const char* modelFileName, Int32& vertexCount, Int32& indexCount, ModelsStructs::NormalMaModelData** modelPtr);
	static bool loadNormalMapModelFromStaticModel(const char* modelFileName, Int32& vertexCount, Int32& indexCount, ModelsStructs::NormalMaModelData** modelPtr);
private:
	static void calculateTangentBinormal(const ModelsStructs::BasicModelData&  vertex1,const ModelsStructs::BasicModelData&  vertex2,const ModelsStructs::BasicModelData&  vertex3,
											_Out_ XMFLOAT3& tangent,_Out_ XMFLOAT3& binormal);
	static void calculateNormal(const XMFLOAT3& tangent,const XMFLOAT3& binormal, _Out_ XMFLOAT3& normal);
	//prevent copy
	GModelLoader(const GModelLoader&);
	GModelLoader& operator= (const GModelLoader&);
};

#endif			//GMODEL_LOADER_H

/*
void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);
*/