#pragma once
#ifndef GABSTRACT_SHADER_H
#define GABSTRACT_SHADER_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <GEngineCore.h>
#include <GMath.h>

using namespace DirectX;
using GMath::GMatrix4x4;
using GMath::GVector4;
using GMath::GVector3;

//--------------------------------------------------------------------------------------
//Class : GAbstractShader
//--------------------------------------------------------------------------------------
class GAbstractShader
{
protected:
	struct MatrixBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};
public:
	GAbstractShader(void);
	virtual ~GAbstractShader(void)									{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd) = 0;		//override me
	void shutDown();

protected:
	virtual void unloadContent() = 0;									//override me

	HRESULT compileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer, HWND hWnd) const;
	void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, char* fileName) const;

	ID3D11VertexShader*		_pVertexShader;
	ID3D11PixelShader*		_pPixelShader;
	ID3D11InputLayout*		_pLayout;
	ID3D11Buffer*			_pMatrixBuffer;
};

#endif	//GABSTRACT_SHADER_H