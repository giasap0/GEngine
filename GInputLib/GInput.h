#pragma once
#ifndef GINPUT_H
#define GINPUT_H

#include "GInputIncludes.h"
#include "GGamePad.h"

enum class GKeys: uint32_t
{
	Zero = DIK_0,
	One = DIK_1,Two,Three,Four,Five,Six,Seven,Eight,Nine,
	Q = DIK_Q,
	W,E,R,T,Y,U,I,O,P,
	A = DIK_A,
	S,D,F,G,H,J,K,L,
	Z= DIK_Z, 
	X,C,V,B,N,M,
	Escape = DIK_ESCAPE, Backspace = DIK_BACK, Tab = DIK_TAB, Return = DIK_RETURN,	Space = DIK_SPACE, Del = DIK_DELETE,
	F1 = DIK_F1,
	F2,F3,F4,F5,F6,F7,F8,F9,F10,
	F11 = DIK_F11,
	F12,F13,F14,F15,
	ArrowUP= DIK_UP, ArrowLeft = DIK_LEFT, ArrowRight = DIK_RIGHT, ArrowDown = DIK_DOWN
};

//--------------------------------------------------------------------------------------
//Class GInput: manage the Input system
//--------------------------------------------------------------------------------------
class GInput
{
public:
	GInput(void);
	~GInput(void)			{}

	HRESULT initialize(HINSTANCE instance, HWND hWnd,  int screenWidth, int screenHeight );
	void shutDown();
	//read the current state of the devices into state buffers
	bool update();
	
	bool IsKeyDown(GKeys key);

	void getMouseLocation(_Out_ int& x, _Out_ int& y, _Out_ int& z);
	inline const GCameraCommand& cameraCommand()											{ return _cameraCommand; }
	inline const GPlaneCommand& planeCommand()												{ return _planeCommand;  }
	inline const GGeneralCommands& generalCommand()											{ return _generalCommand; }
	inline bool gamepadConnected()															{ return _gamePad.isConnected(); }

private:
	bool readKeyboard();
	bool readMouse();
	void processInput();
	//prevent copy
	GInput(const GInput&)	{}
	GInput& operator=(const GInput&);

private:
	IDirectInput8*			_pDirectInput;
	IDirectInputDevice8*	_pKeyboard;
	IDirectInputDevice8*	_pMouse;
	unsigned char			_keyboardState[256];
	DIMOUSESTATE			_mouseState;
	GGamePad				_gamePad;

	int _screenWidth, _screenHeight;
	int _mouseX, _mouseY, _mouseZ;
	GGeneralCommands _generalCommand;
	GCameraCommand _cameraCommand;
	GPlaneCommand _planeCommand;
	bool _isMouseConfigured;
};

#endif	//GINPUT_H