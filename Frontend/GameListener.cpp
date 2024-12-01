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

void GameListener::OnEnableUndoMove(bool isEnabled)
{
	emit enableUndoButton(isEnabled);
}

void GameListener::OnEnableSwapTileMove(bool isEnabled)
{
	emit enableSwapTileButton(isEnabled);
}
