#include "ConsoleGameListener.h"

void game::ConsoleGameListener::OnMoveDone()
{
	std::cout << "Movement is done.";
}

void game::ConsoleGameListener::OnGameOver()
{
	std::cout << "Game is done.";
}

void game::ConsoleGameListener::OnGameReset()
{
	std::cout << "Game got reset.";
}

void game::ConsoleGameListener::OnEnableUndoMove(bool isEnabled)
{
	std::cout << "Undo move is enabled : " << std::boolalpha << isEnabled << ".";
}

void game::ConsoleGameListener::OnEnableSwapTileMove(bool isEnabled)
{
	std::cout << "Swap tiles move is enabled : " << std::boolalpha << isEnabled << ".";
}
