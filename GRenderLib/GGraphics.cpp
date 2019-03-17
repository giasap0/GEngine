#include "GRenderIncludes.h"
#include "GGraphics.h"


GGraphics::GGraphics(void) : _pRenderer(nullptr), _pRendererToTexture(nullptr), _pRenderWindow(nullptr),
							_pCamera(nullptr), _pLight(nullptr) ,_pFrustum(nullptr), _pScene(nullptr),
							_pTextSprite(nullptr) ,_pFont(nullptr),
							_pTextureShader(nullptr), _pLightShader(nullptr), _pModelShader(nullptr),
							_pSprite(nullptr), _pModel(nullptr) 
{
	_mouseX = _mouseY = 0;
	_screenWidth = 1000; _screenHeight = 500;
}

void GGraphics::shutDown()
{
	//models
	if(_pModel!= nullptr)
	{
		_pModel->shutDown();
		delete _pModel;
		_pModel = nullptr;
	}
	if(_pSprite!= nullptr)
	{
		_pSprite->shutDown();
		delete _pSprite;
		_pSprite = nullptr;
	}
	//shaders
	if( _pModelShader != nullptr)
	{
		_pModelShader->shutDown();
		delete _pModelShader;
		_pModelShader = nullptr;
	}
	if(_pLightShader!= nullptr)
	{
		_pLightShader->shutDown();
		delete _pLightShader;
		_pLightShader = nullptr;
	}
	if(_pTextureShader!= nullptr)
	{
		_pTextureShader->shutDown();
		delete _pTextureShader;
		_pTextureShader = nullptr;
	}
	//core
	if(_pFont!= nullptr)
	{
		_pFont->shutDown();
		delete _pFont;
		_pFont = nullptr;
	}
	if(_pTextSprite != nullptr)
	{
		delete _pTextSprite;
		_pTextSprite = nullptr;
	}
	if(_pScene != nullptr)
	{
		_pScene->shutDown();
		delete _pScene;
		_pScene = nullptr;
	}
	if(_pFrustum != nullptr)
	{
		_pFrustum->shutdown();
		delete _pFrustum;
		_pFrustum = nullptr;
	}
	if(_pLight!= nullptr)
	{
		delete _pLight;
		_pLight = nullptr;
	}
	
	if(_pCamera!= nullptr)
	{
		delete _pCamera;
		_pCamera = nullptr;
	}
	if( _pRenderWindow != nullptr)
	{
		_pRenderWindow->shutDown();
		delete _pRenderWindow;
		_pRenderWindow = nullptr;
	}
	if(_pRendererToTexture!= nullptr)
	{
		_pRendererToTexture->shutDown();
		delete _pRendererToTexture;
		_pRendererToTexture = nullptr;
	}
	if(_pRenderer!= nullptr)
	{
		_pRenderer->shutDown();
		delete _pRenderer;
		_pRenderer = nullptr;
	}
}

bool GGraphics::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	_mouseX = _mouseY = 0;
	_screenWidth = screenWidth; _screenHeight=screenHeight;

	//------------------------------------------------------------------------------------------
	// Init Direct3D
	//------------------------------------------------------------------------------------------
	_pRenderer = new GRenderer();
	if(_pRenderer == nullptr)	{return false;}

	HRESULT res = _pRenderer->initialize(screenWidth, screenHeight, g_IsVsyncEnabled, hwnd,
											g_IsFullScreen,g_SCREEN_DEPTH, g_SCREEN_NEAR);
	if( FAILED( res) )
	{
		DBG_OUTPT("Could not initialize Direct3D");	MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	//------------------------------------------------------------------------------------------
	// Init Render To Texture
	//-----------------------------------------------------------------------------------------
	_pRendererToTexture = new GRenderToTexture();
	if(_pRendererToTexture == nullptr)		{return false;}

	bool result = _pRendererToTexture->initialize(_pRenderer->getDevice(), screenWidth, screenHeight);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the Render-To-Texture object");
		MessageBox(hwnd, "Could not initialize the ender-To-Texture object.", "Error", MB_OK);
		return false;
	}

	_pRenderWindow = new GRenderWindow();
	if( _pRenderWindow == nullptr )			{return false;}

	result = _pRenderWindow->initialize(_pRenderer->getDevice(), screenWidth,screenHeight,500,250);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the Render-To-Window object");
		MessageBox(hwnd, "Could not initialize the ender-To-Window object.", "Error", MB_OK);
		return false;
	}

	//------------------------------------------------------------------------------------------
	// Init Camera
	//------------------------------------------------------------------------------------------
	_pCamera = new GCamera();
	if(_pCamera == nullptr)		{return false;}

	_pCamera->setPosition(0.0f,0.0f,-5.0f);
	_pCamera->render();

	//------------------------------------------------------------------------------------------
	// Init the light Object
	//------------------------------------------------------------------------------------------
	_pLight = new GLight();
	if(_pLight==nullptr)		{return false;}

	//_pLight->setAmbientColor(0,0,0,0);						//set ambient light off
	//_pLight->setAmbientColor(1,1,1,1);						//set ambient light on
	_pLight->setAmbientColor(0.5f,0.5f,0.5f,1.0f);
	//_pLight->setDiffuseColor(1.0f,1.0f,1.0f,1.0f);
	_pLight->setDirection(1.0f,0.0f,.0f);
	_pLight->setDiffuseColor(0.0f,0.0f,0.0f,0.0f);		//set beacon of light off

	//------------------------------------------------------------------------------------------
	// Init Frustum
	//------------------------------------------------------------------------------------------
	_pFrustum = new GFrustum();
	if(_pFrustum == nullptr)	{return false;}

	_pFrustum->constructFrustum(g_SCREEN_DEPTH,_pRenderer->getProjectionMatrix() , _pCamera->getViewMatrix() );

	//------------------------------------------------------------------------------------------
	// Init Scene (information about all the models in the scene)
	//-----------------------------------------------------------------------------------------
	_pScene = new GScene();
	if(_pScene == nullptr)		{return false;}

	result = _pScene->initialize(25);
	if(!result)
	{
		DBG_OUTPT("Could not initialize Scene");
		MessageBox(hwnd, "Could not initialize Scene.", "Error", MB_OK);
		return false;
	}

	//------------------------------------------------------------------------------------------
	// Init Text and Font
	//------------------------------------------------------------------------------------------
	const wchar_t* fontFileName = L"fonts/italic.spritefont";
	_pTextSprite = new SpriteBatch(_pRenderer->getDeviceContext() );
	_pFont = new GFont();
	result = _pFont->initialize(_pRenderer->getDevice(),fontFileName);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the font. Filename " << fontFileName);
		MessageBox(hwnd, "Could not initialize thefont", "Error", MB_OK);
		return false;
	}
//------------------------------------------------------------------------------------------
//Shaders
//-----------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	// Init the texture shader
	//------------------------------------------------------------------------------------------
	_pTextureShader = new GTextureShader();
	if(_pTextureShader==nullptr)	{return false;}

	result = _pTextureShader->initialize(_pRenderer->getDevice(),hwnd);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the texture shader object");
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}
	//------------------------------------------------------------------------------------------
	// Init the light shader
	//------------------------------------------------------------------------------------------
	_pLightShader= new GLightShader();
	if(_pLightShader== nullptr)		{return false;}

	result = _pLightShader->initialize(_pRenderer->getDevice(), hwnd);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the light shader object");
		MessageBox(hwnd, "Could not initialize the light shader object.", "Error", MB_OK);
		return false;
	}
	//------------------------------------------------------------------------------------------
	// Init the model shader
	//------------------------------------------------------------------------------------------
	_pModelShader = new GModelShader();
	if(_pModelShader == nullptr)	{return false;}

	result = _pModelShader->initialize(_pRenderer->getDevice(), hwnd);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the model shader object");
		MessageBox(hwnd, "Could not initialize the model shader object.", "Error", MB_OK);
		return false;
	}


//------------------------------------------------------------------------------------------
//Models
//-----------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	// Init Sprites
	//------------------------------------------------------------------------------------------
	_pSprite = new GSprite();
	if(_pSprite == nullptr)			{return false;}

	result = _pSprite->initialize(_pRenderer->getDevice(),screenWidth,screenHeight,"textures/sprite2.dds",200,200);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the sprite object");
		MessageBox(hwnd, "Could not initialize the sprite object.", "Error", MB_OK);
		return false;
	}

	//------------------------------------------------------------------------------------------
	// Init Models
	//------------------------------------------------------------------------------------------
	_pModel = new GModel();
	if(_pModel == nullptr)		{return false;	}

	result = _pModel->initialize(_pRenderer->getDevice(),"models/predator.obj","textures/seafloor.dds" );
	if(!result)
	{
		DBG_OUTPT("Could not initialize the model object");
		MessageBox(hwnd, "Could not initialize the model object. Predator.obj", "Error", MB_OK);
		return false;
	}

	return true;
}

void GGraphics::updateState(float dt, int mouseX, int mouseY)
{
	//------------------------------------------------------------------------------------------
	// Update Variables
	//---------------------------------------------------------------------------------------
	_mouseX = mouseX;	_mouseY= mouseY;
	//static GMath::Radians rotation;
	static const float omega = 360.0f/10;
	//static const float v =-1.0000001f;
	//static float scale=1;
	//rotation += GMath::Degree( omega*dt);
	//scale *= v;
	
	//------------------------------------------------------------------------------------------
	// Update Camera
	//-----------------------------------------------------------------------------------------
	//_pCamera->setRotation(GMath::Radians(),rotation,GMath::Radians());
	//Camera->move(0,0,v*dt);

	//------------------------------------------------------------------------------------------
	// Update Matrices
	//---------------------------------------------------------------------------------------
	GMatrix4x4 worldMatrix = _pRenderer->getWorldMatrix();
	worldMatrix.applyRotation( GMath::Radians(.0f),GMath::Degree(omega*dt), GMath::Radians() );
	//worldMatrix.addTranslation( GVector3(0,0,v*dt) ); 
	//worldMatrix.scale(scale);
	_pRenderer->setWorldMatrix(worldMatrix);
}

bool GGraphics::render()
{
	static const GMatrix4x4 identity = GMatrix4x4::Identity();
	//generate the view matrix
	_pCamera->render();		

	//create the texture for the window
	bool result = renderToTexture();
	if(!result)					{return false;}

	//clear the buffers
	_pRenderer->beginScene(.0f,.0f,.0f,1.0f);

	//render the scene to the back buffer
	result = renderScene();
	if(!result)					{return false;}

	//set Z Buffer Off, render the window we want, and paint it with texture shader using texture from pRendererToTexture 
	_pRenderer->turnZBufferOff();

	/*
	int winPosX = _screenWidth-_pRenderWindow->getWindowWidth()-20;
		result = _pRenderWindow->render(_pRenderer->getDeviceContext(),winPosX,20);
	if(!result)					{return false;}
	result = _pTextureShader->render(_pRenderer->getDeviceContext(),_pRenderWindow->getIndexCount(),
									identity,_pCamera->getViewMatrix(),_pRenderer->getOrthoMatrix(),
									_pRendererToTexture->getShaderResourceView() );
	*/
	result = renderText();
	if(!result)					{return false;}

	_pRenderer->turnZBufferOn();

	_pRenderer->endScene();								//present the rendered scene to screen
	return true;
}

bool GGraphics::renderToTexture()
{
	/*
	// Set the render target to be the render to texture.
	_pRendererToTexture->setRenderTarget( _pRenderer->getDeviceContext() , _pRenderer->getDepthStencilView() );

	// Clear the render to texture.
	GColor clearColor(.0f,.0f,1.0f,.0f);	//i make it transparent
	_pRendererToTexture->clearRenderTarget( _pRenderer->getDeviceContext(), _pRenderer->getDepthStencilView(), clearColor);

	//start drawing
	_pModelNormalMap->render( _pRenderer->getDeviceContext() );
	
	bool result = _pNormalMapShader->render( _pRenderer->getDeviceContext(), _pModelNormalMap->getIndexCount(),
										_pRenderer->getWorldMatrix(),_pCamera->getViewMatrix(), _pRenderer->getProjectionMatrix(),
										_pModelNormalMap->getTextureArray(),
										_pLight->getDirection(),_pLight->getDiffuseColor(), _pLight->getAmbientColor() );
										
	if(!result)					{return false;}	

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	_pRenderer->setBackBufferRenderTarget();
	*/
	return true;
}

bool GGraphics::renderScene()
{
	bool result = true;

	//------------------------------------------------------------------------------------------
	// Get Matrices
	//------------------------------------------------------------------------------------------
	GMath::GMatrix4x4 viewMatrix = _pCamera->getViewMatrix();				
	GMath::GMatrix4x4 worldMatrix=  _pRenderer->getWorldMatrix();
	GMath::GMatrix4x4 projectionMatrix= _pRenderer->getProjectionMatrix();
	GMath::GMatrix4x4 orthoMatrix= _pRenderer->getOrthoMatrix();
	static const GMath::GMatrix4x4 identityMatrix = GMath::GMatrix4x4::Identity();

	//------------------------------------------------------------------------------------------
	// Construct the Frustum
	//-----------------------------------------------------------------------------------------
	_pFrustum->constructFrustum(g_SCREEN_DEPTH,projectionMatrix,viewMatrix);

	//------------------------------------------------------------------------------------------
	// Draw 3D Objects
	//------------------------------------------------------------------------------------------
	
	uint renderCount = 0;
	uint modelCount = _pScene->getModelCount();
	GPoint position;
	GColor color;
	Real radius = _pModel->getRadius();
	bool isModelToRender = false;
	// Go through all the models and render them only if they can be seen by the camera view.
	for( uint i=0; i<modelCount; i++)
	{
		_pScene->getData(i,position,color);
		//check if the model is in the view frustum
		isModelToRender = _pFrustum->checkSphere(position,radius);
		if( isModelToRender)
		{
			worldMatrix.addTranslation( position.getPosition() );
			_pModel->render( _pRenderer->getDeviceContext() );
			result = _pModelShader->render( _pRenderer->getDeviceContext(), _pModel->getIndexCount(),
									worldMatrix,viewMatrix,projectionMatrix,_pModel->getTexture(),
									_pLight->getDirection(), _pLight->getDiffuseColor(), _pLight->getAmbientColor() );
											
			if(result)
				renderCount++;
			else
				DBG_OUTPT("GGraphics::render() - cannot render the model id = " << i);
			//reset the world matrix
			worldMatrix = _pRenderer->getWorldMatrix();
		}
	}		//end for(modelcount)

	return true;
}

bool GGraphics::renderText()
{
	_pTextSprite->Begin(DirectX::SpriteSortMode_Deferred);
	
	GWString string = L" FPS = "; string += GWString::number(_fps);
	_pFont->DrawString(_pTextSprite,string,GMath::GVector2(.0f , 10.0f),0.5f, GColors::Red);

	string = L"Camera position = "; 
	string += GWString( _pCamera->getPosition().toString().const_data() );
	_pFont->DrawString(_pTextSprite,string,GMath::GVector2(.0f , 30.0f),0.5f, GColors::Red);

	_pTextSprite->End();
	return true;
}
