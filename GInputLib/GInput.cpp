#include "GInput.h"

GInput::GInput()  : _pDirectInput(nullptr), _pKeyboard(nullptr), _pMouse(nullptr)
{
}

void GInput::shutDown()
{
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
	_mouseX=0; _mouseY=0;

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
		DBG_OUTPT("GInput: keyboard acquired");
	//------------------------------------------------------------------------------------------
	// Initialize the mouse
	//-----------------------------------------------------------------------------------------
	result = _pDirectInput->CreateDevice(GUID_SysMouse, &_pMouse, nullptr);
	if(FAILED(result) )
	{
		DBG_OUTPT("GInput::initialize Failed creating mouse device ");
		DBG_OUTPT("error HRESULT = " << result );
		return result;
	}

	result = _pMouse->SetDataFormat(&c_dfDIMouse);
	if( FAILED(result) )			return result;

	// Set the cooperative level of the mouse to share with other programs.
	result = _pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if( FAILED(result) )			return result;

	result =  _pMouse->Acquire();
	if(FAILED (result) )
	{
		DBG_OUTPT("GInput::initialize Failed acquiring mouse");
		DBG_OUTPT("error HRESULT = " << result );
		return result;
	}
	else
		DBG_OUTPT("GInput: mouse acquired");

	return S_OK;
}

bool GInput::update()
{
	bool result = readKeyboard();
	if(!result)			return false;

	result = readMouse();
	if(!result)			return false;

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
	//update the location of the mouse
	_mouseX+=_mouseState.lX;
	_mouseY+=_mouseState.lY;

	if(_mouseX<0)				{_mouseX=0;}
	if(_mouseY<0)				{_mouseY=0;}

	if(_mouseX>_screenWidth)	{_mouseX = _screenWidth;}
	if(_mouseY>_screenHeight)	{_mouseY=_screenHeight;}
}

bool GInput::IsKeyDown(GKeys key)
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(_keyboardState[static_cast<int>(key)] & 0x80)
	{
		return true;
	}

	return false;
}

void GInput::getMouseLocation(_Out_ int& x, _Out_ int& y)
{
	x = _mouseX;
	y = _mouseY;
}


/*
bool GInput::IsKeyDown(GKeys key)
{
	int keyIndex = static_cast<int>(key);
	return _keys[keyIndex];
}


private:
	bool readKeyboard();
	bool readMouse();
	void processInput();

	IDirectInput8*			_pDirectInput;
	IDirectInputDevice8*	_pKeyboard;
	IDirectInputDevice8*	_pMouse;
	unsigned char			_keyboardState[256];
	DIMOUSESTATE			_mouseState;

	int _screenWidth, _screenHeight;
	int _mouseX, _mouseY;
};


*/