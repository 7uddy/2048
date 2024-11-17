#pragma once

class IGameListener
{
public:

	//Tells the listener that the board needs to be updated on the interface after move. (Update score too maybe)
	virtual void OnMoveDone() = 0;

	//Tells the listener that the game is over. (Update score for the last time maybe)
	virtual void OnGameOver() = 0;

	virtual ~IGameListener() = default;
};