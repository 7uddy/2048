#pragma once
#include <iostream>
#include "IGameListener.h"

namespace game
{
	class ConsoleGameListener : public IGameListener
	{
	public:
		void OnMoveDone() override;
		void OnGameOver() override;
		void OnGameReset() override;
		void OnEnableUndoMove(bool isEnabled) override;
		void OnEnableSwapTileMove(bool isEnabled) override;
	};
}