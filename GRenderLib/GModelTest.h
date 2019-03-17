#pragma once
#ifndef GMODELTEST_H
#define GMODELTEST_H

//--------------------------------------------------------------------------------------
//Questa è una classe temporanea, per far test su cose che implementerò in GModel
//-------------------------------------------------------------------------------------
#include "GModelLoader.h"
#include "GTextureArray.h"

class GModelTest
{
private:
	struct VertexData
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
public:
	GModelTest(void);
	virtual ~GModelTest(void)			{}

	bool initialize(ID3D11Device* d3dDevice, const char* modelFileName, const char* texture1FileName, const char* texture2FileName);
	void shutDown();
	void render(ID3D11DeviceContext* d3dDeviceCont);

	inline GMath::Int32 getIndexCount()								{return _indexCount;}
	inline ID3D11ShaderResourceView** getTextureArray()				{return _pTextureArray->getTextureArray();}

private:
	bool initializeBuffers(ID3D11Device* d3dDevice);
	void shutDownBuffers();
	void renderBuffers(ID3D11DeviceContext* d3dDeviceCont);

	bool loadTextures(ID3D11Device* device, const char*file1Name, const char* file2Name);
	void releaseTextures();

	bool loadModel(const char* modelFileName);
	void releaseModel();

	GMath::Int32 _vertexCount, _indexCount;
	ID3D11Buffer* _pVertexBuffer, *_pIndexBuffer;
	ModelsStructs::BasicModelData* _pModel;
	GTextureArray* _pTextureArray;

	//prevent copy
	GModelTest(const GModelTest&);
	GModelTest& operator= (const GModelTest&);
};

#endif		//GMODELTEST_H
