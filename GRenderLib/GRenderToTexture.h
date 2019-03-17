#pragma once
#ifndef GRENDERTOTEXTURE_H
#define GRENDERTOTEXTURE_H

#include <d3d11.h>
#include "Shaders\GColors.h"
#include <GMathCore.h>

using GMath::Int32;

//-----------------------------------------------------------------------------------------
//Class GRenderToTexture: allow to render scenes into a texture instead of the back buffer
//----------------------------------------------------------------------------------------
class GRenderToTexture
{
public:
	GRenderToTexture(void);
	virtual ~GRenderToTexture(void)			{}

	bool initialize(ID3D11Device* device, Int32 textureWidth, Int32 textureHeight);
	void shutDown();

	void setRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* dStencilView);
	void clearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* dStencilView, const GColor& clearColor);
	
	inline ID3D11ShaderResourceView* getShaderResourceView()			{return _pShaderResourceView;}

private:
	ID3D11Texture2D* _pRenderTargetTexture;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11ShaderResourceView* _pShaderResourceView;

	//prevent copy
	GRenderToTexture(const GRenderToTexture&);
	GRenderToTexture& operator= (const GRenderToTexture&);
};


#endif	//GRENDERTOTEXTURE_H
