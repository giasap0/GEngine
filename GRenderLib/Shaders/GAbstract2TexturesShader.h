#pragma once
#ifndef GABSTRACT_2TEXTURES_SHADER_H
#define GABSTRACT_2TEXTURES_SHADER_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include "GAbstractShader.h"

// ** all shaders that use 2 textures inherit from this ** //
class GAbstract2TexturesShader : public GAbstractShader
{
public:
	GAbstract2TexturesShader(void);
	virtual ~GAbstract2TexturesShader(void)								{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd) = 0;

	bool render(ID3D11DeviceContext* deviceContext, int indexCount,
				const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
				ID3D11ShaderResourceView**  textureArray);

protected:
	virtual void unloadContent();

	bool initializeShader(ID3D11Device* device, HWND hWnd, char* vsFileName, char* psFileName);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext,
							const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
							ID3D11ShaderResourceView**  textureArray);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);


	ID3D11SamplerState*		_pSampleState;

private:
	//prevent copy
	GAbstract2TexturesShader(const GAbstract2TexturesShader&);
	GAbstract2TexturesShader& operator=(const GAbstract2TexturesShader&);
};

#endif		//GABSTRACT_2TEXTURES_SHADER_H