#include "GRenderIncludes.h"
#include "GGraphics.h"

using GPhysics::GAirplaneStatus;


GGraphics::GGraphics(void) : _pRenderer(nullptr), _pRendererToTexture(nullptr), _pRenderWindow(nullptr),
							_pCamera(nullptr), _pLight(nullptr) ,_pFrustum(nullptr), _pScene(nullptr),
							_pSpriteRenderer(nullptr) ,_pFont(nullptr),
							_pLightShader(nullptr), _pModelNormalMap(nullptr),
							_pSpriteTrimScale(nullptr), _pSpriteTrimArrow(nullptr), _pModel(nullptr), _pNormalMapShader(nullptr)
{
	_mouseX = _mouseY = 0;
	_screenWidth = 1000; _screenHeight = 500;
}

void GGraphics::shutDown()
{
	//models
	if (_pModelNormalMap != nullptr)
	{
		_pModelNormalMap->shutDown();
		delete _pModelNormalMap;
		_pModelNormalMap = nullptr;
	}
	if(_pModel!= nullptr)
	{
		_pModel->shutDown();
		delete _pModel;
		_pModel = nullptr;
	}
	if (_pSpriteTrimArrow != nullptr)
	{
		_pSpriteTrimArrow->shutDown();
		delete _pSpriteTrimArrow;
		_pSpriteTrimArrow = nullptr;
	}
	if (_pSpriteTrimScale != nullptr)
	{
		_pSpriteTrimScale->shutDown();
		delete _pSpriteTrimScale;
		_pSpriteTrimScale = nullptr;
	}
	//shaders
	if(_pNormalMapShader != nullptr)
	{
		_pNormalMapShader->shutDown();
		delete _pNormalMapShader;
		_pNormalMapShader = nullptr;
	}
	if(_pLightShader!= nullptr)
	{
		_pLightShader->shutDown();
		delete _pLightShader;
		_pLightShader = nullptr;
	}
	//core
	if(_pFont!= nullptr)
	{
		_pFont->shutDown();
		delete _pFont;
		_pFont = nullptr;
	}
	if(_pSpriteRenderer != nullptr)
	{
		delete _pSpriteRenderer;
		_pSpriteRenderer = nullptr;
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
	// Init Render Sprites
	//-----------------------------------------------------------------------------------------
	_pSpriteRenderer = new SpriteBatch(_pRenderer->getDeviceContext());
	if (_pSpriteRenderer == nullptr)
	{
		DBG_OUTPT("Could not initialize the Sprite Render object");
		MessageBox(hwnd, "Could not initialize the Sprite Render object", "Error", MB_OK);
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

	_pCamera->setPosition(-0.5f, 0.0f, 0.05f);
	_pCamera->render();

	//------------------------------------------------------------------------------------------
	// Init the light Object
	//------------------------------------------------------------------------------------------
	_pLight = new GLight();
	if(_pLight==nullptr)		{return false;}

	//_pLight->setAmbientColor(0,0,0,0);						//set ambient light off
	_pLight->setAmbientColor(0.3f,0.3f,0.3f,1.0f);
	_pLight->setDiffuseColor(1.0f,1.0f,1.0f,.05f);
	_pLight->setDirection(-1.0f,0.0f,-.05f);
	//_pLight->setDiffuseColor(0.0f,0.0f,0.0f,0.0f);		//set beacon of light off

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

	result = _pScene->initialize(10);
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
	_pFont = new GFont();
	result = _pFont->initialize(_pRenderer->getDevice(),fontFileName);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the font. Filename " << fontFileName);
		MessageBox(hwnd, "Could not initialize thefont", "Error", MB_OK);
		return false;
	}
	//------------------------------------------------------------------------------------------
	// Init Sprites
	//------------------------------------------------------------------------------------------
	_pSpriteTrimScale = new GSprite();
	if (_pSpriteTrimScale == nullptr)			{ return false; }

	result = _pSpriteTrimScale->initialize(_pRenderer->getDevice(), screenWidth, screenHeight, "textures/trim.dds");
	if (!result)
	{
		DBG_OUTPT("Could not initialize the sprite object for trim");
		MessageBox(hwnd, "Could not initialize the sprite object. Cannot find the trim texture", "Error", MB_OK);
		return false;
	}

	_pSpriteTrimArrow = new GSprite();
	if (_pSpriteTrimArrow == nullptr)			{ return false; }

	result = _pSpriteTrimArrow->initialize(_pRenderer->getDevice(), screenWidth, screenHeight, "textures/trim_selector.dds");
	if (!result)
	{
		DBG_OUTPT("Could not initialize the sprite object for trim (arrow)");
		MessageBox(hwnd, "Could not initialize the sprite object. Cannot find the trim texture", "Error", MB_OK);
		return false;
	}

//------------------------------------------------------------------------------------------
//Shaders
//-----------------------------------------------------------------------------------------

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
	// Init the normal map shader
	//------------------------------------------------------------------------------------------
	_pNormalMapShader = new GNormalMapShader();
	if(_pNormalMapShader == nullptr)	{return false;}

	result = _pNormalMapShader->initialize(_pRenderer->getDevice(), hwnd);
	if(!result)
	{
		DBG_OUTPT("Could not initialize the normal map shader object");
		MessageBox(hwnd, "Could not initialize the normal map shader object.", "Error", MB_OK);
		return false;
	}

//------------------------------------------------------------------------------------------
//Models
//-----------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	// Init Models
	//------------------------------------------------------------------------------------------
	_pModel = new GModel();
	if(_pModel == nullptr)		{return false;	}

	result = _pModel->initialize(_pRenderer->getDevice(),"models/ground.mdlS","textures/stoneGrass.dds" );
	if(!result)
	{
		DBG_OUTPT("Could not initialize the model object");
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	//------------------------------------------------------------------------------------------
	// Init Normal Map Model
	//------------------------------------------------------------------------------------------
	_pModelNormalMap = new GModelNormalMap();
	if (_pModelNormalMap == nullptr)		{return false;}

	result = _pModelNormalMap->initialize(_pRenderer->getDevice(),"models/airplane.mdlS", "textures/stoneWall.dds", "textures/normalMapStoneWall.dds");
	if(!result)
	{
		DBG_OUTPT("Could not initialize the normal map Model object");
		MessageBox(hwnd, "Could not initialize the normal map Model object.", "Error", MB_OK);
		return false;
	}

	return true;
}

void GGraphics::updateState(float dt, int mouseX, int mouseY, int mouseZ, const GCameraCommand& cameraCommands, const GAirplaneStatus& planeStatus)
{
	//------------------------------------------------------------------------------------------
	// Update Variables
	//-----------------------------------------------------------------------------------------
	_airplaneLastStatus = planeStatus;
	_mouseX = mouseX;	_mouseY = mouseY; _mouseZ = mouseZ;
	_pScene->setAirplane(planeStatus.position);

	static GMath::Radians rotationV, rotationH;
	static const float omegaCamera = GMath::Constants::pi_double / 10;
	static const float v_mouseZoom = 500.0000001f;
	rotationV = - cameraCommands.cameraVertical*omegaCamera /3560.0f;		//  = - cameraCommands.cameraVertical*omegaCamera*dt;
	rotationH = cameraCommands.cameraHorizontal*omegaCamera  / 3560.0f; //= cameraCommands.cameraHorizontal*omegaCamera * dt;

	//------------------------------------------------------------------------------------------
	// Create String description
	//------------------------------------------------------------------------------------------
	//*******************************************************************************************************************************
	//******************************************************** good for debug ********************************************************
	airplaneStateDescription = L"altitude  = ";	
	airplaneStateDescription += GWString::number((int)planeStatus.position.translationVector().z()) + L" meters\n";
	airplaneStateDescription += L"speed = "; airplaneStateDescription += GWString::number ( (int)planeStatus.v.norm () );
	airplaneStateDescription += L" m/s\n angles= { ";
	airplaneStateDescription += GWString( Degree(planeStatus.aroundX).toString().const_data()) + L" , ";
	airplaneStateDescription += GWString( Degree(planeStatus.aroundY).toString().const_data()) + L" , ";
	airplaneStateDescription += GWString( Degree(planeStatus.aroundZ).toString().const_data()) + L" }\n";
	airplaneStateDescription += L"\n";
	airplaneStateDescription += L" alfa = ";
	airplaneStateDescription += GWString(Degree(planeStatus.alfa).toString().const_data()) + L" , beta = ";
	airplaneStateDescription += GWString(Degree(planeStatus.beta).toString().const_data()) + L" , ";
	airplaneStateDescription += L"\n";
	airplaneStateDescription += L" speed = "; airplaneStateDescription += GWString(planeStatus.v.toString().const_data());
	airplaneStateDescription += L" acc = "; airplaneStateDescription += GWString(planeStatus.a.toString().const_data());
	airplaneStateDescription += L"\n";
	airplaneStateDescription += L" lift = "; airplaneStateDescription += GWString(planeStatus.lift.toString().const_data());
	airplaneStateDescription += L" weight = "; airplaneStateDescription += GWString(planeStatus.weight.toString().const_data());
	airplaneStateDescription += L"\n";
	airplaneStateDescription += L" drag = "; airplaneStateDescription += GWString(planeStatus.drag.toString().const_data());
	airplaneStateDescription += L" thrust = "; airplaneStateDescription += GWString(planeStatus.thrust.toString().const_data());
	airplaneStateDescription += L"\n throttle = ";
	airplaneStateDescription += GWString::number( (int)(planeStatus.commands.throttle * 100) ) + L" %\n";
	airplaneStateDescription += L"\n";
	airplaneStateDescription += L" w_dot = "; airplaneStateDescription += GWString(planeStatus.w_dot.toString().const_data());
	airplaneStateDescription += L" w = "; airplaneStateDescription += GWString(planeStatus.w.toString().const_data());
	airplaneStateDescription += L"\n";
	airplaneStateDescription += L" mx = "; airplaneStateDescription += GWString(planeStatus.mx.toString().const_data());
	airplaneStateDescription += L" my = "; airplaneStateDescription += GWString(planeStatus.my.toString().const_data());
	airplaneStateDescription += L" mz = "; airplaneStateDescription += GWString(planeStatus.mz.toString().const_data());	
	
	// ********************************************************************************************************************************/

	/*******************************************************************************************************************************
	airplaneStateDescription = L"altitude  = ";	
	airplaneStateDescription += GWString::number((int)planeStatus.position.translationVector().z()) + L" meters\n";
	airplaneStateDescription += L"speed = "; airplaneStateDescription += GWString::number((int)planeStatus.v.norm()) + L" m/s, vertical speed = ";
	airplaneStateDescription += GWString::number((int)planeStatus.v.z()) + L" m/s\n";
	airplaneStateDescription += L"alfa = ";
	airplaneStateDescription += GWString::number((int)(Degree(planeStatus.alfa).toReal()) ) + L" deg , beta = ";
	airplaneStateDescription += GWString::number((int)(Degree(planeStatus.beta).toReal() ) ) + L" deg\nheading = ";
	airplaneStateDescription += GWString::number((int)(Degree(planeStatus.aroundZ).toReal() ) ) + L" deg\n";
	airplaneStateDescription += L"throttle = ";
	airplaneStateDescription += GWString::number( (int)(planeStatus.commands.throttle * 100) ) + L" %\n";
	********************************************************************************************************************************/
	
	//------------------------------------------------------------------------------------------
	// Update Camera
	//-----------------------------------------------------------------------------------------
	//
	GVector3 deltaCamera = (planeStatus.v*dt);
	_pCamera->applyRotation(GMath::Radians(), -rotationV, rotationH);
	deltaCamera.x() += _mouseZ*v_mouseZoom/ 3560.0f;// _mouseZ*v_mouseZoom*dt;
	deltaCamera.y() += (_pCamera->getPosition().x() - planeStatus.position.x()) * GMath::sin(rotationH);
	deltaCamera.z() += (_pCamera->getPosition().x() - planeStatus.position.x() ) * GMath::sin(rotationV);
	

	_pCamera->move(deltaCamera);
	

	//------------------------------------------------------------------------------------------
	// Update Matrices
	//---------------------------------------------------------------------------------------
	GMatrix4x4 worldMatrix = _pRenderer->getWorldMatrix();
	//worldMatrix.applyRotation(rotationV, rotationH, GMath::Radians() );
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

	static const GColor sky = GColors::SkyBlue;
	//clear the buffers
	_pRenderer->beginScene(sky.red, sky.green, sky.blue, sky.alpha);

	//render the scene to the back buffer
	result = renderScene();
	if(!result)					{return false;}

	//render sprites and text

	result = renderSprite();
	if (!result)				{ return false; }

	result = renderText();
	if(!result)					{return false;}

	_pRenderer->turnZBufferOn();

	_pRenderer->endScene();								//present the rendered scene to screen
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
	// Draw 3D Static Objects
	//------------------------------------------------------------------------------------------
	
	GVector3 planePosition;
	GMatrix3x3 modelRotation;
	GColor color = GColors::White;
	float radius = 1.0f;
	bool isModelToRender = false;

	// airplane
	_pScene->getAirplane(planePosition, modelRotation);
	//check if the model is in the view frustum
	isModelToRender = true; // _pFrustum->checkSphere(modelPosition, radius);
	if( isModelToRender)
	{
		worldMatrix.addTranslation(planePosition);
		worldMatrix.applyRotation(modelRotation);
		_pModelNormalMap->render(_pRenderer->getDeviceContext());

		result = _pNormalMapShader->render(_pRenderer->getDeviceContext(), _pModelNormalMap->getIndexCount(),
											worldMatrix, viewMatrix, projectionMatrix, _pModelNormalMap->getTextureArray(),
											_pLight->getDirection(), _pLight->getDiffuseColor(), _pLight->getAmbientColor());
		if(!result)
			DBG_OUTPT("GGraphics::render() - cannot render the ariplane model");
		//reset the world matrix
		worldMatrix = _pRenderer->getWorldMatrix();
	}

	// ground
	
	GVector3 groundPosition;
	_pScene->getGround(groundPosition);
	
	if (GMath::absV(planePosition.x() - groundPosition.x()) > 50)
		groundPosition.x() += planePosition.x() - groundPosition.x();
	if (GMath::absV(planePosition.y() - groundPosition.y()) > 50)
		groundPosition.y() += planePosition.y() - groundPosition.y();	
	_pScene->setGroundPosition(groundPosition);
	
	worldMatrix.addTranslation(groundPosition);
	_pModel->render(_pRenderer->getDeviceContext());

	result = _pLightShader->render(_pRenderer->getDeviceContext(), _pModel->getIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _pModel->getTexture(),
		_pLight->getDirection(), _pLight->getDiffuseColor(), _pLight->getAmbientColor());
	if (!result)
		DBG_OUTPT("GGraphics::render() - cannot render the ground");
	
	//reset the world matrix
	worldMatrix = _pRenderer->getWorldMatrix();

	return true;
}

bool GGraphics::renderSprite()
{
	_pSpriteRenderer->Begin(DirectX::SpriteSortMode_Deferred);
	_positionXSpriteTrim = _screenWidth - 110.0f;
	_positionYSpriteTrim = _screenHeight / 2.0f;
	_positionYSpriteTrimText = 85.0f;
	float positionElevator = (Degree(_airplaneLastStatus.commands.trimElevator).toReal() / 30.0f) * _positionYSpriteTrimText ;
	_pSpriteTrimScale->render(_pSpriteRenderer, _positionXSpriteTrim, _positionYSpriteTrim, .5f);
	_pSpriteTrimArrow->render(_pSpriteRenderer, _positionXSpriteTrim + 20.0f, _positionYSpriteTrim + _positionYSpriteTrimText + positionElevator, .5f);

	GWString angleTrim = GWString::number(Degree(_airplaneLastStatus.commands.trimElevator).toReal());
	angleTrim = angleTrim.subStr(0, 4); angleTrim += L" deg";
	_pFont->DrawString(_pSpriteRenderer, angleTrim, GMath::GVector2(_positionXSpriteTrim + 40.0f, _positionYSpriteTrim + _positionYSpriteTrimText - 7.0f + positionElevator),
						0.4f, GColors::Red);

	_pFont->DrawString(_pSpriteRenderer, GWString(L"Trim"), GMath::GVector2(_positionXSpriteTrim, _positionYSpriteTrim - 30.0f + positionElevator), 0.4f, GColors::Red);

	_pSpriteRenderer->End();
	return true;
}

bool GGraphics::renderText()
{
	_pSpriteRenderer->Begin(DirectX::SpriteSortMode_Deferred);
	
	GWString stringFPS = L" FPS = "; stringFPS += GWString::number(_fps);
	_pFont->DrawString(_pSpriteRenderer, stringFPS, GMath::GVector2(.0f, 10.0f), 0.5f, GColors::Red);

	_pFont->DrawString(_pSpriteRenderer, airplaneStateDescription, GMath::GVector2(.0f, 30.0f), 0.3f, GColors::Red);
	
//	string = L"Camera position = "; 
//	string += GWString( _pCamera->getPosition().toString().const_data());
//	_pFont->DrawString(_pSpriteRenderer,string,GMath::GVector2(.0f , 200.0f),0.3f, GColors::Red);
	
	_pSpriteRenderer->End();
	return true;
}

// use this for mirror effect
bool GGraphics::renderToTexture()
{
	//TODO rewrite using DirectxTK sprite
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