#include "game.h"

//Game
void usGame::Game::linkInput()
{
	SendMessage(windowHwnd, US_WM_INPUT, 0, reinterpret_cast<LPARAM>(this));
}