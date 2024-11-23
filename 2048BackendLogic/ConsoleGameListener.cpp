#include "ConsoleGameListener.h"

void game::ConsoleGameListener::OnMoveDone()
{
	std::cout << "Movement is done";
}

void game::ConsoleGameListener::OnGameOver()
{
	std::cout << "Game is done";
}
