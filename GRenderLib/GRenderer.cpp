#include "GRenderIncludes.h"
#include "GRenderer.h"
#include <dxgi.h>

using namespace DirectX;


GRenderer::GRenderer(void)	: _pSwapChain(nullptr), _pDevice(nullptr), _pDeviceContext(nullptr),
							_pRenderTargetView(nullptr), _pDepthStencilBuffer(nullptr), _pDepthStencilState(nullptr),
							_pDepthStencilView(nullptr), _pRasterState(nullptr), _pDepthDisabledStencilState(nullptr)
{
}

void GRenderer::shutDown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(_pSwapChain != nullptr)
	{
		_pSwapChain->SetFullscreenState(false,nullptr);
	}
	if(_pDepthDisabledStencilState != nullptr)
	{
		_pDepthDisabledStencilState->Release();
		_pDepthDisabledStencilState = nullptr;
	}
	if(_pRasterState != nullptr)
	{
		_pRasterState->Release();
		_pRasterState = nullptr;
	}
	if(_pDepthStencilView!= nullptr)
	{
		_pDepthStencilView->Release();
		_pDepthStencilView = nullptr;
	}
	if(_pDepthStencilState != nullptr)
	{
		_pDepthStencilState->Release();
		_pDepthStencilState = nullptr;
	}
	if(_pDepthStencilBuffer != nullptr)
	{
		_pDepthStencilBuffer->Release();
		_pDepthStencilBuffer = nullptr;
	}
	if(_pRenderTargetView != nullptr)
	{
		_pRenderTargetView->Release();
		_pRenderTargetView = nullptr;
	}
	if(_pDeviceContext != nullptr)
	{
		_pDeviceContext->Release();
		_pDeviceContext = nullptr;
	}
	if(_pDevice!= nullptr)
	{
		_pDevice->Release();
		_pDevice = nullptr;
	}
	if(_pSwapChain != nullptr)
	{
		_pSwapChain->Release();
		_pSwapChain = nullptr;
	}
}

HRESULT GRenderer::initialize(int screenWidth, int screenHeight, bool IsVsyncEnabled, HWND hwnd,
						bool IsFullScreen, float screenDepth, float screenNear)
{	
	_vsyncEnabled = IsVsyncEnabled;

	//-------------------------------------------------------------------------
	//get the refresh rate from the video card/monitor
	//-------------------------------------------------------------------------
	IDXGIFactory*		factory			= nullptr;
	IDXGIAdapter*		adapter			= nullptr;
	IDXGIOutput*		adapterOutput	= nullptr;
	// Create a DirectX graphics interface factory.
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))					{return result;}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))					{return result;}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))					{return result;}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	unsigned int numModes;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))					{return result;}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if(displayModeList==nullptr)		{return E_FAIL;}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))					{return result;}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	unsigned int numerator, denominator;
	numerator=60;
	denominator=1;
	for(unsigned int i=0; i<numModes; ++i)
	{
		if( (displayModeList[i].Width == (unsigned int)screenWidth)&&(displayModeList[i].Height == (unsigned int)screenHeight) )
		{
				numerator   = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}	//end for

	DBG_OUTPT("refreshRate = " << static_cast<float>(numerator) / static_cast<float>(denominator) );

	//-------------------------------------------------------------------------
	//get the name of the video card and the amount of memory on the video card
	//-------------------------------------------------------------------------
	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))					{return result;}

	// Store the dedicated video card memory in megabytes.
	_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	DBG_OUTPT("video card memory = " << _videoCardMemory);

	// Convert the name of the video card to a character array and store it.
	unsigned int stringLength;
	int error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)						{return E_FAIL;}
	DBG_OUTPT("video card name = " << _videoCardDescription);

	//-------------------------------------------------------------------------
	//release the structures and interfaces used to get video card informations
	//-------------------------------------------------------------------------
	delete [] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	//-------------------------------------------------------------------------
	// Fill out the description of the swap chain
	//-------------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;										// Set to a single back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;						// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// Set regular 32-bit surface for the back buffer.

	if(_vsyncEnabled)													// Set the refresh rate of the back buffer.
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// Set the usage of the back buffer.
	swapChainDesc.OutputWindow = hwnd;									// Set the handle for the window to render to.
	swapChainDesc.SampleDesc.Count = 1;									// Turn multisampling off.
	swapChainDesc.SampleDesc.Quality = 0;

	if(IsFullScreen)													// Set to full screen or windowed mode.
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				// Discard the back buffer contents after presenting.
	swapChainDesc.Flags = 0;											// Don't set the advanced flags.

	//-------------------------------------------------------------------------
	// Select the feature level and driver type, 
	// create the swap chain, Direct3D device, and Direct3D device context.
	//-------------------------------------------------------------------------
	unsigned int creationFlags=0;	//0 for device with no debugging capabilities
		#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

	D3D_DRIVER_TYPE driverTypes[3]=	{D3D_DRIVER_TYPE_HARDWARE , D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[4] = {D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	unsigned int driver=0;
	for(driver=0; driver<totalDriverTypes;++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[driver],NULL,creationFlags,
										featureLevels,totalFeatureLevels,D3D11_SDK_VERSION,
										&swapChainDesc,&_pSwapChain,&_pDevice,
										&_featureLevel, &_pDeviceContext);
										
		if(SUCCEEDED(result))
		{
			_driverType = driverTypes[driver];
			DBG_OUTPT("Succeded creating device and swap chain on driver: " << driver);
			
			switch (_featureLevel)
			{
			case D3D_FEATURE_LEVEL_10_0: DBG_OUTPT("Feature Level = DX 10_0");
				break;
			case D3D_FEATURE_LEVEL_10_1: DBG_OUTPT("Feature Level = DX 10_1");
				break;
			case D3D_FEATURE_LEVEL_11_0: DBG_OUTPT("Feature Level = DX 11_0");
				break;
			case D3D_FEATURE_LEVEL_11_1: DBG_OUTPT("Feature Level = DX 11_1");
				break;
			default: DBG_OUTPT("Feature Level = N.D. ");
				break;
			}
			break;
		}
	}//end for
	if( FAILED( result ) )
	{
		DBG_OUTPT( "Failed to create the Direct3D device!" );
		return result;
	}

	//-------------------------------------------------------------------------
	// get a pointer to the back buffer and attach it to the swap chain
	//-------------------------------------------------------------------------
	ID3D11Texture2D*	backBufferPtr	= nullptr;
	result = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		DBG_OUTPT("Failed to get the swap chain back buffer!" );
		return result;
	}

	// Create the render target view with the back buffer pointer.
	result = _pDevice->CreateRenderTargetView(backBufferPtr, NULL, &_pRenderTargetView);
	if(backBufferPtr!= nullptr)
	{
		backBufferPtr->Release();
		backBufferPtr = nullptr;
	}
	if(FAILED(result))						{return result;}
	
	//-------------------------------------------------------------------------
	// setup a depth buffer description and attach a stencil buffer to it
	//-------------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	// Create the texture for the depth buffer using the filled out description.
	result = _pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &_pDepthStencilBuffer);
	if(FAILED(result))						{return result;}

	//-------------------------------------------------------------------------
	// setup the depth stencil description and create depth stencil state
	//-------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	result = _pDevice->CreateDepthStencilState(&depthStencilDesc, &_pDepthStencilState);
	if(FAILED(result))						{return result;}

	// Set the depth stencil state.
	_pDeviceContext->OMSetDepthStencilState(_pDepthStencilState, 1);

	//-------------------------------------------------------------------------
	// setup the depth stencil with DepthEnable set to false (for 2D drawing)
	//-------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc) );
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	depthDisabledStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	depthDisabledStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	result = _pDevice->CreateDepthStencilState(&depthDisabledStencilDesc,&_pDepthDisabledStencilState);
	if( FAILED(result) )					{return result;}

	//-------------------------------------------------------------------------
	// create the description of the view of the depth stencil buffer
	//-------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = _pDevice->CreateDepthStencilView(_pDepthStencilBuffer, &depthStencilViewDesc, &_pDepthStencilView);
	if(FAILED(result))						{return result;}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	_pDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);

	//-------------------------------------------------------------------------
	// create a rasterizer state. This give control over how polygons are rendered.
	//-------------------------------------------------------------------------
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = _pDevice->CreateRasterizerState(&rasterDesc, &_pRasterState);
	if(FAILED(result))						{return result;}

	// Now set the rasterizer state.
	_pDeviceContext->RSSetState(_pRasterState);

	//-------------------------------------------------------------------------
	// setup and create the viewport
	//-------------------------------------------------------------------------
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	_pDeviceContext->RSSetViewports(1, &viewport);

	//-------------------------------------------------------------------------
	// initialize matrices
	//-------------------------------------------------------------------------
	float fieldOfView = GMath::Constants::pi_quarter;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	XMMATRIX projM = XMMatrixPerspectiveFovLH(fieldOfView,screenAspect,screenNear, screenDepth);
	GMath::storeFromXMMatrix(_projectionMatrix,projM);

	_worldMatrix.makeItIdentity();

	XMMATRIX orthoM = XMMatrixOrthographicLH((float)screenWidth,(float)screenHeight,screenNear, screenDepth);
	GMath::storeFromXMMatrix(_orthoMatrix,orthoM);

 return S_OK;
}


void GRenderer::beginScene(float red, float green, float blue, float alpha)
{
	static float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back and depth buffers.
	_pDeviceContext->ClearRenderTargetView(_pRenderTargetView,color);
	_pDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void GRenderer::endScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(_vsyncEnabled)
	{
		// Lock to screen refresh rate.
		_pSwapChain->Present(1,0);
	}
	else
	{
		// Present as fast as possible.
		_pSwapChain->Present(0, 0);
	}
}


void GRenderer::getVideoCardInfo(char* cardName, int& memory) const
{
	strcpy_s(cardName, 128, _videoCardDescription);
	memory = _videoCardMemory;
	return;
}

