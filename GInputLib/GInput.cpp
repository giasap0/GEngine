#include "GInput.h"
#include <Xinput.h>

GInput::GInput() : _pDirectInput(nullptr), _pKeyboard(nullptr), _pMouse(nullptr)
{
	_isMouseConfigured = false;
}

void GInput::shutDown()
{
	_isMouseConfigured = false;
	if(_pMouse!=nullptr)
	{
		_pMouse->Unacquire();
		_pMouse->Release();
		_pMouse = nullptr;
	}
	if(_pKeyboard != nullptr)
	{
		_pKeyboard->Unacquire();
		_pKeyboard->Release();
		_pKeyboard = nullptr;
	}
	if(_pDirectInput != nullptr)
	{
		_pDirectInput->Release();
		_pDirectInput = nullptr;
	}
	return;
}

HRESULT GInput::initialize(HINSTANCE hinstance, HWND hWnd, int screenWidth, int screenHeight )
{
	// Store the screen size which will be used for positioning the mouse cursor.
	_screenHeight= screenHeight;	_screenWidth = screenWidth;
	_mouseX=0; _mouseY=0, _mouseZ=0;
	_cameraCommand.resetValues();
	_planeCommand.resetValues();
	_generalCommand.resetValues();
	_isMouseConfigured = false;

	//----------------------------------------------------------------------------------------
	//Initialize the main direct input interface
	//----------------------------------------------------------------------------------------
	HRESULT result = DirectInput8Create(hinstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&_pDirectInput,nullptr);
	if(FAILED(result) )
	{
		DBG_OUTPT("GInput::initialize Failed creating main direct input interface ");
		DBG_OUTPT("error HRESULT = " << result );
		return result;
	}

	//------------------------------------------------------------------------------------------
	// Initialize the keyboard
	//------------------------------------------------------------------------------------------
	result = _pDirectInput->CreateDevice(GUID_SysKeyboard,&_pKeyboard,nullptr);
	if(FAILED(result) )
	{
		DBG_OUTPT("GInput::initialize Failed creating keyboard device ");
		DBG_OUTPT("error HRESULT = " << result );
		return result;
	}
	result = _pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if( FAILED(result) )		return result;

	// Set the cooperative level of the keyboard to not share with other programs.
	result = _pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if( FAILED(result) )		return result;

	result = _pKeyboard->Acquire();
	if(FAILED (result) )
	{
		DBG_OUTPT("GInput::initialize Failed acquiring keyboard");
		DBG_OUTPT("error HRESULT = " << result );
		return result;
	}
	else
	{
		DBG_OUTPT("GInput: keyboard acquired");
	}
	
	//------------------------------------------------------------------------------------------
	// Initialize the mouse
	//-----------------------------------------------------------------------------------------
	result = _pDirectInput->CreateDevice(GUID_SysMouse, &_pMouse, nullptr);
	if(FAILED(result) )
	{
		DBG_OUTPT("GInput::initialize Failed creating mouse device ");
		DBG_OUTPT("error HRESULT = " << result );
		_isMouseConfigured = false;
	}

	result = _pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)){
		_isMouseConfigured = false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = _pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)){
		_isMouseConfigured = false;
	}

	result =  _pMouse->Acquire();
	if(FAILED (result) )
	{
		DBG_OUTPT("GInput::initialize Failed acquiring mouse");
		DBG_OUTPT("error HRESULT = " << result );
		_isMouseConfigured = false;
	}
	else
	{
		DBG_OUTPT("GInput: mouse acquired");
		_isMouseConfigured = true;
	}

	//------------------------------------------------------------------------------------------
	// Initialize the gamePad
	//-----------------------------------------------------------------------------------------
	if (!_gamePad.initialize())
	{
		DBG_OUTPT("GInput: gamePad not acquired");
	}		
	else
	{
		DBG_OUTPT("GInput: gamePad acquired");
	}

	return S_OK;
}

bool GInput::update()
{
	bool result = readKeyboard();
	if(!result)			return false;

	if (_isMouseConfigured)
	{
		result = readMouse();
		if (!result)			return false;
	}

	_gamePad.updateState();

	processInput();
	return true;
}

bool GInput::readKeyboard()
{
	HRESULT result = _pKeyboard->GetDeviceState(sizeof(_keyboardState),(LPVOID)&_keyboardState);
	if( FAILED(result) )
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_pKeyboard->Acquire();
		}
		else
		{
			DBG_OUTPT("GInput::readKeyboard() an error occour trying to read keyboard state. error = " << result);
			return false;
		}
	}
	return true;
}

bool GInput::readMouse()
{
	HRESULT result = _pMouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&_mouseState);
	if(FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_pMouse->Acquire();
		}
		else
		{
			DBG_OUTPT("GInput::readMouse() an error occour trying to read mouse state. error = " << result);
			return false;
		}
	}

	return true;
}

void GInput::processInput()
{
	_cameraCommand.resetValues();
	_planeCommand.resetValues();

	//update the location of the mouse
	_mouseX+=_mouseState.lX;
	_mouseY+=_mouseState.lY;
	_mouseZ=_mouseState.lZ/120;

	if(_mouseX<0)				{_mouseX=0;}
	if(_mouseY<0)				{_mouseY=0;}

	if(_mouseX>_screenWidth)	{_mouseX = _screenWidth;}
	if(_mouseY>_screenHeight)	{_mouseY=_screenHeight;}

	//------------------------------------------------------------------------------------------
	// General
	//-----------------------------------------------------------------------------------------
	if (this->IsKeyDown(GKeys::Escape))
	{
		_generalCommand.quit() = true;
	}
	if (this->IsKeyDown(GKeys::P))
	{
		_generalCommand.changePauseStatus();
	}

	//------------------------------------------------------------------------------------------
	// Camera
	//-----------------------------------------------------------------------------------------
	if (IsKeyDown(GKeys::A))
		_cameraCommand.cameraHorizontal += 1;
	if (IsKeyDown(GKeys::D))
		_cameraCommand.cameraHorizontal -= 1;
	if (IsKeyDown(GKeys::W))
		_cameraCommand.cameraVertical += 1;	
	if (IsKeyDown(GKeys::S))
		_cameraCommand.cameraVertical -= 1;

	//------------------------------------------------------------------------------------------
	// Airplane
	//-----------------------------------------------------------------------------------------
	if (IsKeyDown(GKeys::ArrowLeft))
		_planeCommand.roll() = -1;
	else if (IsKeyDown(GKeys::ArrowRight))
		_planeCommand.roll() = +1;
	else
		_planeCommand.roll() = 0;

	if (IsKeyDown(GKeys::ArrowUP))
		_planeCommand.pitch() = 1.0;
	else if (IsKeyDown(GKeys::ArrowDown))
		_planeCommand.pitch() = -1.0;
	else
		_planeCommand.pitch() = 0;

	if (IsKeyDown(GKeys::R))
		_planeCommand.yaw() = 1;
	else if (IsKeyDown(GKeys::E))
		_planeCommand.yaw() = -1;
	else
		_planeCommand.yaw() = 0;

	if (IsKeyDown(GKeys::O))
		_planeCommand.trimElevator() = 1;
	else if (IsKeyDown(GKeys::L))
		_planeCommand.trimElevator() = -1;
	else
		_planeCommand.trimElevator() = .0f;

	if (IsKeyDown(GKeys::T))
		_planeCommand.throttle() = 1;
	else if (IsKeyDown(GKeys::G))
		_planeCommand.throttle() = -1;
	else
		_planeCommand.throttle() = .0f;

	_planeCommand.isAnalogicCommand() = false;
	//------------------------------------------------------------------------------------------
	//  gamePad - check connected
	//-----------------------------------------------------------------------------------------
	if (!_gamePad.isConnected())
		return;

	_planeCommand.isAnalogicCommand() = true;
	//------------------------------------------------------------------------------------------
	// General - gamePad
	//-----------------------------------------------------------------------------------------
	if (_gamePad.IsButtonDown(GPad_Keys::BACK))
		_generalCommand.quit() = true;
	if (_gamePad.IsButtonDown(GPad_Keys::START))
		_generalCommand.changePauseStatus();

	//------------------------------------------------------------------------------------------
	// Airplane - gamePad
	//-----------------------------------------------------------------------------------------
	_planeCommand.roll() = _gamePad.rightStick_X();
	_planeCommand.pitch() = _gamePad.rightStick_Y();
	_planeCommand.yaw() = _gamePad.rightTrigger();
	_planeCommand.yaw() -= _gamePad.leftTrigger();

	if (_gamePad.IsButtonDown(GPad_Keys::Y))
		_planeCommand.throttle() = 1;
	else if (_gamePad.IsButtonDown(GPad_Keys::A))
		_planeCommand.throttle() = -1;
	else
		_planeCommand.throttle() = .0f;

	if (_gamePad.IsButtonDown(GPad_Keys::DIR_PAD_UP))
		_planeCommand.trimElevator() = 1;
	else if (_gamePad.IsButtonDown(GPad_Keys::DIR_PAD_DOWN))
		_planeCommand.trimElevator() = -1;
	else
		_planeCommand.trimElevator() = .0f;
	//------------------------------------------------------------------------------------------
	// Camera - gamePad
	//-----------------------------------------------------------------------------------------
	_cameraCommand.cameraHorizontal =- _gamePad.leftStick_X();
	_cameraCommand.cameraVertical = _gamePad.leftStick_Y();

}//EO processInput

bool GInput::IsKeyDown(GKeys key)
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(_keyboardState[static_cast<int>(key)] & 0x80)
	{
		return true;
	}

	return false;
}

void GInput::getMouseLocation(_Out_ int& x, _Out_ int& y, _Out_ int& z)
{
	x = _mouseX;
	y = _mouseY;
	z = _mouseZ;
}
