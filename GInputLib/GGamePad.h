#pragma once
#ifndef GGAME_PAD_H
#define GGAME_PAD_H

#include <Windows.h>
#include <Xinput.h>

enum class GPad_Keys : WORD
{
	//ACTIONS BUTTONS

	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,

	//OTHERS

	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	L_THUMB = XINPUT_GAMEPAD_LEFT_THUMB,
	R_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB,	

	//DIRECTIONAL PAD
	DIR_PAD_UP = XINPUT_GAMEPAD_DPAD_UP,
	DIR_PAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	DIR_PAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	DIR_PAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT	
};

//--------------------------------------------------------------------------------------
//Class GGamePad: represent an XINPU GamePad (like the Xbox controller )
//--------------------------------------------------------------------------------------
class GGamePad
{
public:
	GGamePad()								{ _connected = false; _LX = _LY = _RX = _RY = _LTrigger = _RTrigger = 0.0f; }
	virtual ~GGamePad()						{}

	inline unsigned int  getIndex() const	{ return _padIndex; }
	inline bool isConnected() const			{ return _connected; }
	// Return X axis of left stick (normalized to [-1, +1] )
	inline float leftStick_X() const		{ return _LX; }
	// Return Y axis of left stick (normalized to [-1, +1] )
	inline float leftStick_Y() const		{ return _LY; }
	// Return X axis of right stick (normalized to [-1, +1] )
	inline float rightStick_X() const		{ return _RX; }
	// Return Y axis of right stick (normalized to [-1, +1] )
	inline float rightStick_Y() const		{ return _RY; }

	// Return left trigger (normalized to [0, +1] )
	inline float leftTrigger() const		{ return _LTrigger; }
	// Return right trigger (normalized to [0, +1] )
	inline float rightTrigger() const		{ return _RTrigger; }

	// Vibrate the gamepad (0.0f cancel, 1.0f max speed)
	//when he starts rumble then you have to recall this function with (.0f) to make joypad sto vibrating
	void vibrate(float a_fLeftMotor = .0f, float a_fRightMotor = .0f) const;

	bool IsButtonDown(GPad_Keys key) const;
	
	//try to find one gamePad connected
	bool initialize();
	const XINPUT_STATE& getState();
	//update state and return true if gamePad is connected
	bool updateState();

	// - Return true if stick is inside deadzone, false if outside
	bool LStick_InDeadzone() const;
	bool RStick_InDeadzone() const;
private:

private:
	XINPUT_STATE _state;
	unsigned int _padIndex;
	bool _connected;

	float _LX, _LY, _RX, _RY, _LTrigger, _RTrigger;

private:
	//prevent copy
	GGamePad(const GGamePad&);
	GGamePad& operator=(const GGamePad&);
};

#endif	//GGAME_PAD_H