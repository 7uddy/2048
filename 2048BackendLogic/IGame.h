#pragma once
#include "IBoard.h"
#include <string>
#include "IGameListener.h"

namespace game
{
	class IGame
	{
	public:
		virtual void Move(Movement position) = 0;
		virtual void SetBoard(const std::string& board) = 0;
		virtual std::string GetBoard() const = 0;

		virtual void AddListener(IGameListenerPtr observer) = 0;
		virtual void RemoveListener(IGameListener* observer) = 0;

	private:
		virtual void MoveRowOrColumnWithData(unsigned int startIndex, unsigned int endIndex, int increaseFactorForStart) = 0;
	};
}