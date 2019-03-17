#pragma once
#ifndef GCOLOR_SHADER_H
#define GCOLOR_SHADER_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include "GAbstractShader.h"

//--------------------------------------------------------------------------------------
//Class : GColorShader
//--------------------------------------------------------------------------------------
class GColorShader	: public GAbstractShader
{
public:
	GColorShader(void);
	GColorShader(const GColorShader&)														{}
	virtual ~GColorShader(void)																{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd);

	bool render(ID3D11DeviceContext* deviceContext, int indexCount,
				const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection);

protected:
	virtual void unloadContent();

private:

	bool initializeShader(ID3D11Device* device, HWND hWnd, char* vsFileName, char* psFileName);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext,
							const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};

#endif	//GCOLOR_SHADER_H