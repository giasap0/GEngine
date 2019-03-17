#pragma once
#ifndef GRENDERER_H
#define GRENDERER_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <D3Dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <GMath.h>

using DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;
using GMath::GMatrix4x4;

//--------------------------------------------------------------------------------------
//Class GRenderer: create and manage the directX window
//--------------------------------------------------------------------------------------
class GRenderer
{
public:
	GRenderer(void);
	GRenderer(const GRenderer&)		{}
	~GRenderer(void)				{}

	HRESULT initialize(int screenWidth, int screenHeight, bool IsVsyncEnabled, HWND hwnd,
						bool IsFullScreen, float screenDepth, float screenNear);
	void shutDown();

	//initializes the buffers so they are blank and ready to be drawn to
	void beginScene(float red, float green, float blue, float alpha);
	//tells the swap chain to display scene once all the drawing has completed at the end of each frame
	void endScene();

	inline ID3D11Device*			getDevice()				{return _pDevice;}
	inline ID3D11DeviceContext*		getDeviceContext()		{return _pDeviceContext;}

	inline const GMatrix4x4& getProjectionMatrix() const						{return _projectionMatrix;}
	inline const GMatrix4x4& getWorldMatrix() const								{return _worldMatrix;}
	inline const GMatrix4x4& getOrthoMatrix() const								{return _orthoMatrix;}

	inline void setProjectionMatrix(const GMatrix4x4& projectionM)				{_projectionMatrix = projectionM;}
	inline void setWorldMatrix(const GMatrix4x4& worldM)						{_worldMatrix = worldM;}
	inline void setOrthoMatrix(const GMatrix4x4& orthoM)						{_orthoMatrix = orthoM;}

	void getVideoCardInfo(char* cardName, int& memory) const;

	inline void turnZBufferOn()						{_pDeviceContext->OMSetDepthStencilState(_pDepthStencilState,1);}
	inline void turnZBufferOff()					{_pDeviceContext->OMSetDepthStencilState(_pDepthDisabledStencilState,1);}

	inline ID3D11DepthStencilView* getDepthStencilView()					{return _pDepthStencilView;}
	inline void setBackBufferRenderTarget()									{_pDeviceContext->OMSetRenderTargets(1,&_pRenderTargetView,_pDepthStencilView);}

private:
	GMatrix4x4					_projectionMatrix;
	GMatrix4x4					_worldMatrix;
	GMatrix4x4					_orthoMatrix;

	D3D_FEATURE_LEVEL			_featureLevel;
	D3D_DRIVER_TYPE				_driverType;

	bool						_vsyncEnabled;
	int							_videoCardMemory;
	char						_videoCardDescription[128];

	IDXGISwapChain*				_pSwapChain;
	ID3D11Device*				_pDevice;
	ID3D11DeviceContext*		_pDeviceContext;
	ID3D11RenderTargetView*		_pRenderTargetView;
	ID3D11Texture2D*			_pDepthStencilBuffer;
	ID3D11DepthStencilState*	_pDepthStencilState;
	ID3D11DepthStencilView*		_pDepthStencilView;
	ID3D11RasterizerState*		_pRasterState;
	ID3D11DepthStencilState*	_pDepthDisabledStencilState;
};

#endif	//GRENDERER_H