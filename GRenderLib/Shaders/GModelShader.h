#pragma once
#ifndef GMODEL_SHADER_H
#define GMODEL_SHADER_H

#include "GAbstractShader.h"
#include "GColors.h"

/*
enum class GShaderTarget : uint8_t
{
	Target_VertexShader	= 1,
	Target_PixelShader,
	Target_GeometryShader,
	Target_HullShader,
	Target_DomainShader
};
*/

class GModelShader : public GAbstractShader
{
protected:
	struct LightBufferType
	{
		GVector4 ambientColor;
		GVector4 diffuseColor;
		GVector3 lightDirection;
		float padding;
	};

public:
	GModelShader();
	virtual ~GModelShader()			{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd);

	bool render(ID3D11DeviceContext* deviceContext, GMath::Int32 indexCount,
				const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
				ID3D11ShaderResourceView* texturePtr,
				const GVector3& lightDirection, const GColor& diffuseColor,const GColor& ambientColor);

protected:
	virtual void unloadContent();

private:
	bool initializeShader(ID3D11Device* device, HWND hWnd, char* vsFileName, char* psFileName);
	//geometrySFileName,char* domainSFileName,char* hullSFileName

	bool setShaderParameters(ID3D11DeviceContext* deviceContext,
							const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
							ID3D11ShaderResourceView* texturePtr,
							const GVector3& lightDirection, const GColor& diffuseColor,const GColor& ambientColor);
	void renderShader(ID3D11DeviceContext* deviceContext, GMath::Int32 indexCount, GMath::Int32 startIndex=0);

	ID3D11SamplerState*		_pSampleState;
	ID3D11Buffer*			_pLightBuffer;
	/*
	ID3D11GeometryShader* geometryShader;
	ID3D11HullShader* hullShader;
	ID3D11DomainShader* domainShader;
	*/

private:
	//prevent copy
	GModelShader(const GModelShader&);
	GModelShader& operator=(const GModelShader&);
};



#endif	//GMODEL_SHADER_H