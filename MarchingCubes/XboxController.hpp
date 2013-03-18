#ifndef __XBOXCONTROLLER_HPP__
#define __XBOXCONTROLLER_HPP__

#include <iostream>
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

using namespace std;


class XboxController
{
private:
	bool			isConnected;

public:
	DWORD				player;
	XINPUT_STATE		currState;
	XINPUT_STATE		prevState;
	XINPUT_VIBRATION	vibration;

	float leftThumbStickX;
	float leftThumbStickY;
	float rightThumbStickX;
	float rightThumbStickY;
	float leftTrigger;
	float rightTrigger;

	XboxController()
	{
		XboxController(0);
	}

	XboxController(unsigned int player)
	{
		ZeroMemory(&currState, sizeof(XINPUT_STATE));
		ZeroMemory(&prevState, sizeof(XINPUT_STATE));
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

		this->isConnected = false;
		this->player = player;

		this->leftThumbStickX = this->leftThumbStickY = 0.0f;
		this->rightThumbStickX = this->rightThumbStickY = 0.0f;
		this->leftTrigger = this->rightTrigger = 0.0f;
	}

	~XboxController(){}

	bool IsConnected()
	{
		return isConnected;
	}

	void Update()
	{
		prevState = currState;
		
		DWORD result;
		ZeroMemory(&currState, sizeof(XINPUT_STATE));

		result = XInputGetState(0, &currState);
		
		if (result != ERROR_SUCCESS)
		{
			cout << "controller was disconnected!\n";
			isConnected = false;
			return;
		}
		
		this->leftThumbStickX = this->currState.Gamepad.sThumbLX;
		this->leftThumbStickY = this->currState.Gamepad.sThumbLY;
		this->rightThumbStickX = this->currState.Gamepad.sThumbRX;
		this->rightThumbStickY = this->currState.Gamepad.sThumbRY;

		if (this->leftThumbStickX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && this->leftThumbStickX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			this->leftThumbStickY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && this->leftThumbStickY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			this->leftThumbStickX = this->leftThumbStickY = 0;
		}
		else
		{
			this->leftThumbStickX /= 32767.0f;
			this->leftThumbStickY /= 32767.0f;
		}
		if (this->rightThumbStickX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && this->rightThumbStickX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			this->rightThumbStickY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && this->rightThumbStickY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			this->rightThumbStickX = this->rightThumbStickY = 0;
		}
		else
		{
			this->rightThumbStickX /= 32767.0f;
			this->rightThumbStickY /= 32767.0f;
		}

		this->leftTrigger = this->currState.Gamepad.bLeftTrigger;
		this->rightTrigger = this->currState.Gamepad.bRightTrigger;

		if (this->leftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			this->leftTrigger = 0.0f;
		if (this->rightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			this->rightTrigger = 0.0f;
	}
};

#endif