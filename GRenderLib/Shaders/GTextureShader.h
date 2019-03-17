#pragma once
#ifndef GTEXTURE_SHADER_H
#define GTEXTURE_SHADER_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include "GAbstractShader.h"

//--------------------------------------------------------------------------------------
//Class : GTextureShader
//--------------------------------------------------------------------------------------
class GTextureShader : public GAbstractShader
{
public:
	GTextureShader(void);
	virtual ~GTextureShader(void)									{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd);

	bool render(ID3D11DeviceContext* deviceContext, int indexCount,
				const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
				ID3D11ShaderResourceView* texturePtr);

protected:
	virtual void unloadContent();

private:
	bool initializeShader(ID3D11Device* device, HWND hWnd, char* vsFileName, char* psFileName);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext,
							const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
							ID3D11ShaderResourceView* texturePtr);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);


	ID3D11SamplerState*		_pSampleState;

	//prevent copy
	GTextureShader(const GTextureShader&);
	GTextureShader& operator=(const GTextureShader&);
};

#endif	//GTEXTURE_SHADER_H
