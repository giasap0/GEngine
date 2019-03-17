#include "GRenderToTexture.h"
#include <GEngineCore.h>


GRenderToTexture::GRenderToTexture(void) : _pRenderTargetTexture(nullptr), _pRenderTargetView(nullptr), _pShaderResourceView(nullptr)
{
}

bool GRenderToTexture::initialize(ID3D11Device* device, Int32 textureWidth, Int32 textureHeight)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc) );
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//create the render target texture
	HRESULT result = device->CreateTexture2D(&textureDesc,nullptr,&_pRenderTargetTexture);
	if( FAILED(result) )
	{
		DBG_OUTPT("GRenderToTexture::initialize - failed creating render target texture2D");
		return false;
	}

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(_pRenderTargetTexture, &renderTargetViewDesc, &_pRenderTargetView);
	if(FAILED(result))
	{
		DBG_OUTPT("GRenderToTexture::initialize - failed creating render target view");
		return false;
	}

	// Setup the description of the shader resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;	
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(_pRenderTargetTexture, &shaderResourceViewDesc, &_pShaderResourceView);
	if(FAILED(result))
	{
		DBG_OUTPT("GRenderToTexture::initialize - failed creating the shader resource view.");
		return false;
	}

	return true;
}

void GRenderToTexture::shutDown()
{
	if ( _pShaderResourceView != nullptr)
	{
		_pShaderResourceView->Release();
		_pShaderResourceView = nullptr;
	}
	if( _pRenderTargetView != nullptr )
	{
		_pRenderTargetView->Release();
		_pRenderTargetView = nullptr;
	}
	if(_pRenderTargetTexture != nullptr)
	{
		_pRenderTargetTexture->Release();
		_pRenderTargetTexture = nullptr;
	}
}

void GRenderToTexture::setRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* dStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	context->OMSetRenderTargets(1, &_pRenderTargetView,dStencilView);
}

void GRenderToTexture::clearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* dStencilView, const GColor& clearColor)
{
	float color[4];
	color[0] = clearColor.red;
	color[1] = clearColor.green;
	color[2] = clearColor.blue;
	color[3] = clearColor.alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(_pRenderTargetView,color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(dStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
