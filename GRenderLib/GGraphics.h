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
//shaders
#include "Shaders\GTextureShader.h"
#include "Shaders\GLightShader.h"
#include "Shaders\GModelShader.h"

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

	void updateState(float dt, int mouseX, int mouseY);
	bool render();

	inline void setFps(int fps)	{_fps=fps;}

private:
	bool renderToTexture();
	bool renderScene();
	bool renderText();

private:
	GRenderer*			_pRenderer;
	GRenderToTexture*	_pRendererToTexture;
	GRenderWindow*		_pRenderWindow;
	GCamera*			_pCamera;
	GLight*				_pLight;
	GFrustum*			_pFrustum;
	GScene*				_pScene;
	SpriteBatch*		_pTextSprite;
	GFont*				_pFont;
	//shaders
	GTextureShader*		_pTextureShader;
	GLightShader*		_pLightShader;
	GModelShader*		_pModelShader;

	//models
	GSprite*			_pSprite;
	GModel*				_pModel;

	int _mouseX, _mouseY, _fps;
	int _screenWidth, _screenHeight;

	//prevent copy
	GGraphics(const GGraphics&);
	GGraphics& operator=(const GGraphics&);
};

#endif	//GGRAPHICS_H

