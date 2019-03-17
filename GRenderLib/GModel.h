#pragma once
#ifndef GMODEL_H
#define GMODEL_H

//--------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------
#include "GTexture.h"
#include <GEngineCore.h>
#include <GVector3.h>
#include <DirectXMath.h>

using GMath::GVector3;

//--------------------------------------------------------------------------------------------------------------------
//Class : GModel : every instance put a model vertex on the pipeline (use render(ID3D11DeviceContext* d3dDeviceCont);
//Can load from file .Obj
//--------------------------------------------------------------------------------------------------------------------
class GModel
{
private:
	struct VertexData
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

public:
	GModel(void);
	virtual ~GModel(void)			{}

	//handle initializing and shutdown of the model's vertex and index buffers

	bool initialize(ID3D11Device* d3dDevice, const char* modelFileName, const char* textureFileName);
	void shutDown();

	// Puts the model geometry on the video card to prepare it for drawing by the color shader.
	void render(ID3D11DeviceContext* d3dDeviceCont);
	void normalize(GMath::Real scaleTo = 1.0f, bool center = true);

	// Getter methods.

	inline GVector3			getCenter() const					{return _center;}
	inline GMath::Real		getWidth() const					{return _width;}
	inline GMath::Real		getHeight() const					{return _height;}
	inline GMath::Real		getLength() const					{return _length;}
	inline GMath::Real		getRadius() const					{return _radius;}

	inline GMath::Int32		getIndexCount() const				{return _indexCount;}
	inline GMath::Int32		getVertexCount() const				{return _vertexCount;}
	inline ID3D11ShaderResourceView*	getTexture()			{return _pTexture->getTexture();}

private:
	bool initializeBuffers(ID3D11Device* d3dDevice);
	void shutDownBuffers();
	void renderBuffers(ID3D11DeviceContext* d3dDeviceCont);
	
	bool loadTexture(ID3D11Device* device, const char* textFileName);
	void releaseTexture();
	bool loadModel(const char* modelFileName);
	void releaseModel();

	void calculateBounds(_Out_ GVector3& center,
						_Out_ GMath::Real& width,_Out_ GMath::Real& height,
						_Out_ GMath::Real& length,_Out_ GMath::Real& radius) const;
	void scale(GMath::Real scaleFactor, const GVector3& offset = GVector3() );

private:
	GMath::GVector3 _center;
	GMath::Real _width;
    GMath::Real _height;
    GMath::Real _length;
    GMath::Real _radius;

	ID3D11Buffer* _pVertexBuffer, *_pIndexBuffer;
	GTexture* _pTexture;
	VertexData* _pModel;
	GMath::Int32 _vertexCount, _indexCount;

	//prevent copy
	GModel(const GModel&);
	GModel& operator= (const GModel&);
};

#endif	//GMODEL_H
