#pragma once
#ifndef GMODEL_NORMALMAP_H
#define GMODEL_NORMALMAP_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <DirectXMath.h>
#include <GMath.h>
#include "GTextureArray.h"
#include "GModelLoader.h"



//---------------------------------------------------------------------------------------------------------------------
//Class GModelNormalMap : represent a model with a color texture and a normal map. Use normalMapShader to render
//---------------------------------------------------------------------------------------------------------------------
class GModelNormalMap
{
private:
	struct VertexData
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};
public:
	GModelNormalMap(void);
	virtual ~GModelNormalMap(void)									{}

	//handle initializing and shutdown of the model's vertex and index buffers

	bool initialize(ID3D11Device* d3dDevice, const char* modelFileName, const char* textureFileName, const char* normalMapFileName);
	void shutDown();

	// Puts the model geometry on the video card to prepare it for drawing by the color shader.
	void render(ID3D11DeviceContext* d3dDeviceCont);

	inline Int32						getIndexCount() const		{return _indexCount;}
	inline ID3D11ShaderResourceView**	getTextureArray()			{return _pTextures->getTextureArray();}

private:
	bool initializeBuffers(ID3D11Device* d3dDevice);
	void shutDownBuffers();
	void renderBuffers(ID3D11DeviceContext* d3dDeviceCont);
	
	bool loadTextures(ID3D11Device* device, const char* textFileName, const char* normalMapFileName);
	void releaseTextures();
	bool loadModel(const char* modelFileName);
	void releaseModel();
	
	ID3D11Buffer* _pVertexBuffer, *_pIndexBuffer;
	GTextureArray* _pTextures;
	ModelsStructs::NormalMaModelData* _pModel;
	Int32 _vertexCount, _indexCount;

	//prevent copy
	GModelNormalMap(const GModelNormalMap&);
	GModelNormalMap& operator= (const GModelNormalMap&);
};

#endif	GMODEL_NORMALMAP_H