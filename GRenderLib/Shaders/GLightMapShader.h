#pragma once
#ifndef GLIGHTMAP_SHADER_H
#define GLIGHTMAP_SHADER_H

#include "GAbstract2TexturesShader.h"

// ** apply a color texture and then apply light built from a light map ** //
class GLightMapShader : public GAbstract2TexturesShader
{
public:
	GLightMapShader(void);
	virtual ~GLightMapShader(void)				{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd);

private:

	//prevent copy
	GLightMapShader(const GLightMapShader&);
	GLightMapShader& operator= (const GLightMapShader&);
};


#endif		//GLIGHTMAP_SHADER_H


/*
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
*/