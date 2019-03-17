#pragma once
#ifndef GMODEL_H
#define GMODEL_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include "GModelLoader.h"
#include "GTexture.h"


//--------------------------------------------------------------------------------------------------------------------
//Class : GModel : every instance put a model vertex on the pipeline (use render(ID3D11DeviceContext* d3dDeviceCont);
//--------------------------------------------------------------------------------------------------------------------
class GModel
{
private:
	struct VertexData
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
public:
	GModel(void);
	~GModel(void)													{}

	//handle initializing and shutdown of the model's vertex and index buffers

	bool initialize(ID3D11Device* d3dDevice, const char* modelFileName, const char* textureFileName);
	void shutDown();

	// Puts the model geometry on the video card to prepare it for drawing by the color shader.
	void render(ID3D11DeviceContext* d3dDeviceCont);

	inline int							getIndexCount() const		{return _indexCount;}
	inline ID3D11ShaderResourceView*	getTexture()				{return _pTexture->getTexture();}

private:
	bool initializeBuffers(ID3D11Device* d3dDevice);
	void shutDownBuffers();
	void renderBuffers(ID3D11DeviceContext* d3dDeviceCont);
	
	bool loadTexture(ID3D11Device* device, const char* fileName);
	void releaseTexture();
	bool loadModel(const char* modelFileName);
	void releaseModel();
	
	ID3D11Buffer* _pVertexBuffer, *_pIndexBuffer;
	GTexture* _pTexture;
	ModelsStructs::BasicModelData* _pModel;
	int _vertexCount, _indexCount;

	//prevent copy
	GModel(const GModel&);
	GModel& operator= (const GModel&);
};

#endif	//GMODEL_H