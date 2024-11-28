#include "GameListener.h"

void GameListener::OnMoveDone()
{
	emit notifyMoveDone();
}

void GameListener::OnGameOver()
{
	emit notifyGameOver();
}

void GameListener::OnGameReset()
{
	emit notifyGameReset();
}
