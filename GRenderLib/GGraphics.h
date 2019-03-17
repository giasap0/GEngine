#pragma once
#ifndef GGRAPHICS_H
#define GGRAPHICS_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include "GRenderer.h"
#include "GRenderToTexture.h"
#include "GRenderWindow.h"
#include "GCamera.h"
#include "GLight.h"
#include "GFrustum.h"
#include "GScene.h"
#include "GFont.h"
//models
#include "GSprite.h"
#include "GModel.h"
#include "GModelNormalMap.h"
//shaders
#include "Shaders\GTextureShader.h"
#include "Shaders\GLightShader.h"
#include "Shaders\GNormalMapShader.h"
//commons
#include <GCommands.h>
#include <GPhysic.h>

//--------------------------------------------------------------------------------------
//Globals
//--------------------------------------------------------------------------------------
const bool	g_IsFullScreen		= false;
const bool	g_IsMouseHide		= false;
const bool	g_IsVsyncEnabled	= false;
const float g_SCREEN_DEPTH		= 1000.0f;
const float g_SCREEN_NEAR		= 0.1f;

//--------------------------------------------------------------------------------------
//Class : GGraphics : manage the whole graphic system
//------------------------------------------------------------------------------------
class GGraphics
{
public:
	GGraphics(void);
	~GGraphics(void)			{}

	void shutDown();
	bool initialize(int screenWidth, int screenHeight, HWND hwnd);

	void updateState(float dt, int mouseX, int mouseY, int mouseZ, const GCameraCommand& cameraCommands, const GPhysics::GAirplaneStatus& planeStatus);
	bool render();

	inline void setFps(int fps)	{_fps=fps;}

private:
	bool renderToTexture();
	bool renderScene();
	bool renderText();
	bool renderSprite();

private:
	GRenderer*			_pRenderer;
	GRenderToTexture*	_pRendererToTexture;
	GRenderWindow*		_pRenderWindow;
	GCamera*			_pCamera;
	GLight*				_pLight;
	GFrustum*			_pFrustum;
	GScene*				_pScene;
	SpriteBatch*		_pSpriteRenderer;
	GFont*				_pFont;
	//shaders
	GLightShader*		_pLightShader;
	GNormalMapShader*	_pNormalMapShader;
	//models
	GSprite*			_pSpriteTrimScale;
	GSprite*			_pSpriteTrimArrow;
	GModel*				_pModel;
	GModelNormalMap*	_pModelNormalMap;

	int _mouseX, _mouseY, _mouseZ, _fps;
	int _screenWidth, _screenHeight;
	float _positionXSpriteTrim, _positionYSpriteTrim, _positionYSpriteTrimText;

	GPhysics::GAirplaneStatus _airplaneLastStatus;

	GWString airplaneStateDescription;
	GWString _trimElevatorValue;

	//prevent copy
	GGraphics(const GGraphics&);
	GGraphics& operator=(const GGraphics&);
};

#endif	//GGRAPHICS_H

