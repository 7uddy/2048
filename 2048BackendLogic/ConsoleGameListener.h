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
	};
}