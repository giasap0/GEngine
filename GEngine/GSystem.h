#pragma once
#ifndef GSYSTEM_H
#define GSYSTEM_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <Windows.h>
//systems
#include <GGraphics.h>
#include <GInput.h>
#include <GSound.h>
#include <GCommands.h>
#include <GPhysic.h>
//utility objects
#include <GHighResTimer.h>
#include "GFps.h"

//--------------------------------------------------------------------------------------
// Function Prototypes
//--------------------------------------------------------------------------------------
static LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------------------------------------------
// Class GSystem: takes care of initializing, running and shutDown every system in the game engine
//-------------------------------------------------------------------------------------------------
class GSystem
{
public:
	GSystem(void);
	GSystem(LPCSTR applicationName);
	GSystem(const GSystem&)			{}
	virtual ~GSystem(void)			{}

	void shutDown();
	HRESULT initialize();
	//Application Loop
	void run();

	LRESULT CALLBACK messageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
	HRESULT initWindow(int&screenWidth, int&screenHeight);
	void shutDownWindows();

private:
	LPCSTR		_applicationName;
	HINSTANCE		_hInstance;
	HWND			_hWnd;

	GFps*			_pFps;
	GInput*			_pInput;
	GGraphics*		_pGraphics;
	GSound*			_pSound;
	GPhysics::GPhysic*		_pPhysic;
};

//--------------------------------------------------------------------------------------
//Globals
//--------------------------------------------------------------------------------------
static GSystem* _gpApplicationHandle =nullptr;

#endif	//GSYSTEM_H