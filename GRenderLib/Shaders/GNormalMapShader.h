#pragma once
#ifndef GNORMALMAP_SHADER_H
#define GNORMALMAP_SHADER_H

#include "GAbstractShader.h"
#include "GColors.h"

// ** apply a color texture and then apply light built from a normal map ** //
class GNormalMapShader : public GAbstractShader
{
protected:
	struct LightBufferType
	{
		XMFLOAT4    ambientColor;
		XMFLOAT4	diffuseColor;
		XMFLOAT3	lightDirection;
		float		padding;			// Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};
public:
	GNormalMapShader(void);
	virtual ~GNormalMapShader(void)				{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd);

	bool render(ID3D11DeviceContext* deviceContext, int indexCount,
				const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
				ID3D11ShaderResourceView** textureArray,
				const GVector3& lightDirection, const GColor& diffuseColor,const GColor& ambientColor);

protected:
	virtual void unloadContent();

private:
	bool initializeShader(ID3D11Device* device, HWND hWnd, char* vsFileName, char* psFileName);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext,
							const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
							ID3D11ShaderResourceView** textureArray,
							const GVector3& lightDirection, const GColor& diffuseColor,const GColor& ambientColor);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);


	ID3D11SamplerState*		_pSampleState;
	ID3D11Buffer*			_pLightBuffer;

	//prevent copy
	GNormalMapShader(const GNormalMapShader&);
	GNormalMapShader& operator= (const GNormalMapShader&);
};

#endif	//GNORMALMAP_SHADER_H