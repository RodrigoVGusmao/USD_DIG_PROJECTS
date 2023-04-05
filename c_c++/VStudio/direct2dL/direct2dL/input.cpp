#include "input.h"

#include<windowsx.h>

//keyboard
usInput::GKeyboard::GKeyboard()
{
	keyState.resize(0x100, false);
	textMode = false;
	textMax = 0;
}

void usInput::GKeyboard::updateState(DWORD key, bool pressed)
{
	keyState[key] = pressed;
}

void usInput::GKeyboard::setTextMode(bool mode, size_t textMax)
{
	textMode = mode;
	this->textMax = textMax;

	if (mode == false)
		text.clear();
}

void usInput::GKeyboard::inputText(TCHAR ch)
{
	if (textMode)
		text += ch;
}

bool usInput::GKeyboard::getState(DWORD key)
{
	return keyState[key];
}

bool usInput::GKeyboard::getTextMode()
{
	return textMode;
}

std::basic_string<TCHAR> usInput::GKeyboard::getText()
{
	return text;
}

//mouse
void usInput::GMouse::setMousePos(LPARAM lParam)
{
	mousePos.x = GET_X_LPARAM(lParam);
	mousePos.y = GET_Y_LPARAM(lParam);
}

void usInput::GMouse::setRawMouse(LPARAM lParam)
{
	UINT size = 40;
	static BYTE data[40];

	GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, data, &size, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(data);
	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		rawMouse.x = raw->data.mouse.lLastX;
		rawMouse.y = raw->data.mouse.lLastY;
	}
}

void usInput::GMouse::setMouseWheel(WPARAM wParam)
{
	mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
}

void usInput::GMouse::setMouseXButton(WPARAM wParam)
{
	x1Button = (wParam & MK_XBUTTON1) ? true : false;
	x2Button = (wParam & MK_XBUTTON2) ? true : false;
}

//controller
void usInput::Controller::readControllers(int8_t index)
{
	if (controller[min(index, MAX_CONTROLLERS-1)].connected)
	{
		if (XInputGetState(min(index, MAX_CONTROLLERS-1), &controller[min(index, MAX_CONTROLLERS-1)].state) == ERROR_DEVICE_NOT_CONNECTED)
			controller[min(index, MAX_CONTROLLERS-1)].connected = false;
	}
}

void usInput::Controller::setVibrateLeft(int8_t index, WORD speed, float sec)
{
	controller[min(index, MAX_CONTROLLERS - 1)].vibration.wLeftMotorSpeed = speed;
	controller[min(index, MAX_CONTROLLERS - 1)].leftTimeVibration = sec;
}
void usInput::Controller::setVibrateRight(int8_t index, WORD speed, float sec)
{
	controller[min(index, MAX_CONTROLLERS - 1)].vibration.wRightMotorSpeed = speed;
	controller[min(index, MAX_CONTROLLERS - 1)].rightTimeVibration = sec;
}
void usInput::Controller::vibrateControllers(float frameTime)
{
	for (int8_t i = 0; i < MAX_CONTROLLERS; ++i)
	{
		if (controller[min(i, MAX_CONTROLLERS - 1)].connected)
		{
			float remainingTime = max((controller[min(i, MAX_CONTROLLERS - 1)].leftTimeVibration -= frameTime), 0);
			if (remainingTime == 0)
			{
				controller[min(i, MAX_CONTROLLERS - 1)].vibration.wLeftMotorSpeed = 0;
			}
			remainingTime = max((controller[min(i, MAX_CONTROLLERS - 1)].rightTimeVibration -= frameTime), 0);
			if (remainingTime == 0)
			{
				controller[min(i, MAX_CONTROLLERS - 1)].vibration.wRightMotorSpeed = 0;
			}
			XInputSetState(i, &controller[min(i, MAX_CONTROLLERS - 1)].vibration);
		}
	}
}