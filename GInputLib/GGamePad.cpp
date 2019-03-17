#include "GGamePad.h"
#include "GInputIncludes.h"
#include <math.h>
//#include <Xinput.h>

const float MAXIMUM_STICK_VALUE = 32767.0F;
const float MAXIMUM_TRIGGER_VALUE = 255.0f;
const float MAXIMUM_MOTOR_RUMBLE = 65535.0f;

bool GGamePad::initialize()
{
	DBG_OUTPT("GInput: try to find a gamePad");

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&_state, sizeof(XINPUT_STATE));
		DWORD Result = XInputGetState( i , &_state);

		if (Result != ERROR_SUCCESS)
			_connected = false;
		else
		{
			_connected = true;			
			_padIndex = i;
			DBG_OUTPT("GInput: found gamepad with index == " << _padIndex);
			break;
		}			
	}
	return _connected;
}

const XINPUT_STATE& GGamePad::getState()
{
	ZeroMemory(&_state, sizeof(XINPUT_STATE));

	DWORD Result = XInputGetState(_padIndex, &_state);

	if (Result == ERROR_SUCCESS)
		_connected = true;
	else
		_connected = false;

	return _state;
}

bool GGamePad::updateState()
{
	this->getState();
	if (!_connected)
		return false;

	if (LStick_InDeadzone())
	{
		_LX = _LY = .0f;
	}
	else
	{
		short lx = _state.Gamepad.sThumbLX;
		short ly = _state.Gamepad.sThumbLY;

		//determine how far the controller is pushed
		float magnitudeL = sqrtf(  (float)(lx*lx + ly*ly) );

		//determine the direction the controller is pushed
		_LX = (float)(lx) / magnitudeL;
		_LY = (float)(ly) / magnitudeL;
	}
	if (RStick_InDeadzone())
	{
		_RX = _RY = .0f;
	}
	else
	{
		short rx = _state.Gamepad.sThumbRX;
		short ry = _state.Gamepad.sThumbRY;

		//determine how far the controller is pushed
		float magnitudeL = sqrtf( (float)(rx*rx + ry*ry) );

		//determine the direction the controller is pushed
		_RX = (float)(rx) / magnitudeL;
		_RY = (float)(ry) / magnitudeL;

		_RX = _Pow_int<float>(_RX, 5 );
		_RY = _Pow_int<float>(_RY, 5);
	}

	BYTE trig = _state.Gamepad.bLeftTrigger;
	if (trig > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		_LTrigger = (float)(trig) / MAXIMUM_TRIGGER_VALUE;
	else
		_LTrigger = .0f; // Trigger was not pressed

	trig = _state.Gamepad.bRightTrigger;
	if (trig > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		_RTrigger = (float)(trig) / MAXIMUM_TRIGGER_VALUE;
	else
		_RTrigger = .0f; // Trigger was not pressed

	return _connected;
}

// -----------------------------------------------------------------------------

bool GGamePad::IsButtonDown(GPad_Keys key) const
{
	if (_state.Gamepad.wButtons & static_cast<WORD>(key) )
	{
		return true;
	}
	return false;
}

// Vibrate the gamepad (0.0f cancel, 1.0f max speed)
void GGamePad::vibrate(float a_fLeftMotor, float a_fRightMotor) const
{
	if (!_connected)
		return;
	// Vibration state
	XINPUT_VIBRATION VibrationState;

	// Zero memory
	ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

	// Calculate vibration values
	int iLeftMotor = int(a_fLeftMotor * MAXIMUM_MOTOR_RUMBLE);
	int iRightMotor = int(a_fRightMotor * MAXIMUM_MOTOR_RUMBLE);

	// Set vibration values
	VibrationState.wLeftMotorSpeed = iLeftMotor;
	VibrationState.wRightMotorSpeed = iRightMotor;

	// Set the vibration state
	XInputSetState(_padIndex, &VibrationState);
}

// - Return true if stick is inside deadzone, false if outside
bool GGamePad::LStick_InDeadzone() const
{
	if (!_connected)
		return true;
	//X & Y axes of the stick
	short LX = _state.Gamepad.sThumbLX;
	short LY = _state.Gamepad.sThumbLY;
	
	float magnitude = sqrtf( (float)(LX*LX + LY*LY) );

	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		return false;

	// One (or both axes) axis is inside of deadzone
	return true;
}

bool GGamePad::RStick_InDeadzone() const
{
	if (!_connected)
		return true;
	//X & Y axes of the stick
	short RX = _state.Gamepad.sThumbRX;
	short RY = _state.Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = sqrtf( (float)( RX*RX + RY*RY) );

	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		return false;

	// One (or both axes) axis is inside of deadzone
	return true;
}
