#pragma once
#define WIN32_LEAN_AND_MEAN

#include<windows.h>

#include"graphics.h"
#include"timer.h"
#include"input.h"

namespace usGame
{
	class Game : public usGraphics::WindowD9, public usTimer::FrameRate
	{
	private:
		void* argument;

	public:
		using usGraphics::WindowD9::WindowD9;
		void linkInput();
	};
}
