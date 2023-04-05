#pragma once
#define WIN32_LEAN_AND_MEAN

#include<windows.h>
#include<Xinput.h>

#include<vector>
#include<string>

#include"settings.h"

#ifndef MAX_CONTROLLERS
#define MAX_CONTROLLERS 1
#endif

namespace usInput
{
	class GKeyboard
	{
	private:
		std::vector<bool> keyState;

		std::basic_string<TCHAR> text;
		bool textMode;
		size_t textMax;

	public:
		GKeyboard();

		void updateState(DWORD key, bool pressed);
		void setTextMode(bool mode, size_t textMax = ~0);
		void inputText(TCHAR ch);

		bool getState(DWORD key);
		bool getTextMode();
		std::basic_string<TCHAR> getText();
	};

	class Keyboard : private GKeyboard
	{
		using GKeyboard::GKeyboard;

		bool getState(DWORD key) { return GKeyboard::getState(key); };
		bool getTextMode() { return GKeyboard::getTextMode(); };
		std::basic_string<TCHAR> getText() { return GKeyboard::getText(); };
	};

	class GMouse
	{
	private:
		POINT mousePos;
		POINT rawMouse;
		DWORD mouseWheel;

		bool lButton;
		bool rButton;
		bool mButton;
		bool x1Button;
		bool x2Button;

	public:
		void setMousePos(LPARAM lParam);
		void setRawMouse(LPARAM lParam);
		void setMouseLButton(bool state) { lButton = state; };
		void setMouseRButton(bool state) { rButton = state; };
		void setMouseMButton(bool state) { mButton = state; };
		void setMouseWheel(WPARAM wParam);
		void setMouseXButton(WPARAM wParam);

		POINT getMousePos() { return mousePos; };
		POINT getRawMouse() { return rawMouse; };
		bool getMouseLButton() { return lButton; };
		bool getMouseRButton() { return rButton; };
		bool getMouseMButton() { return mButton; };
		DWORD getMouseWheel() { return mouseWheel; };
		bool getMouseX1Button() { return x1Button; };
		bool getMouseX2Button() { return x2Button; };
	};

	class Mouse : private GMouse
	{
	public:
		POINT getMousePos() { return GMouse::getMousePos(); };
		POINT getRawMouse() { return GMouse::getRawMouse(); };
		bool getMouseLButton() { return GMouse::getMouseLButton(); };
		bool getMouseRButton() { return GMouse::getMouseRButton(); };
		bool getMouseMButton() { return GMouse::getMouseMButton(); };
		DWORD getMouseWheel() { return GMouse::getMouseWheel(); };
		bool getMouseX1Button() { return GMouse::getMouseX1Button(); };
		bool getMouseX2Button() { return GMouse::getMouseX2Button(); };
	};

	struct controllerState {
		XINPUT_STATE state;
		XINPUT_VIBRATION vibration;
		float leftTimeVibration;
		float rightTimeVibration;
		bool connected = false;
	};

	class Controller
	{
	private:
		controllerState controller[MAX_CONTROLLERS];
		DWORD thumbstickDeadZone = static_cast<DWORD>(0.2f * static_cast<float>(MAXWORD >> 1));
		DWORD triggerDeadZone = 30;

	public:
		void readControllers(int8_t index);
		const controllerState* getState(int8_t index, DWORD button)
		{
			return &controller[min(index, MAX_CONTROLLERS-1)];
		}
		const WORD getButtonsState(int8_t index)
		{
			return controller[min(index, MAX_CONTROLLERS-1)].state.Gamepad.wButtons;
		}
		bool testButtonState(int8_t index, WORD button)
		{
			return static_cast<bool>(getButtonsState(index) & button);
		}
		POINTS getLeftThumbstickPos(int8_t index)
		{
			POINTS pos = { controller[min(index, MAX_CONTROLLERS - 1)].state.Gamepad.sThumbLX, controller[min(index, MAX_CONTROLLERS - 1)].state.Gamepad.sThumbLY };
			return pos;
		}
		POINTS getRightThumbstickPos(int8_t index)
		{
			POINTS pos = { controller[min(index, MAX_CONTROLLERS - 1)].state.Gamepad.sThumbRX, controller[min(index, MAX_CONTROLLERS - 1)].state.Gamepad.sThumbRY };
			return pos;
		}
		BYTE getLeftTrigger(int8_t index)
		{
			return controller[min(index, MAX_CONTROLLERS - 1)].state.Gamepad.bLeftTrigger;
		}
		BYTE getRightTrigger(int8_t index)
		{
			return controller[min(index, MAX_CONTROLLERS - 1)].state.Gamepad.bRightTrigger;
		}

		void setVibrateLeft(int8_t index, WORD speed, float sec);
		void setVibrateRight(int8_t index, WORD speed, float sec);
		void vibrateControllers(float frameTime);
	};
}