#pragma once
#include <string>
#include "IBoard.h"
#include "IGameListener.h"

namespace game
{
	class IGame
	{
	public:
		virtual void ApplyMove(Movement position) = 0;

		virtual bool IsGameOver() = 0;

		virtual void SetBoard(const std::string& board) = 0;
		virtual std::string GetBoard() const = 0;

		virtual void AddListener(IGameListenerPtr observer) = 0;
		virtual void RemoveListener(IGameListener* observer) = 0;

	private:
		virtual void PlacePieceAtRandomPosition() = 0;

		virtual void NotifyListenersForMoveDone() const = 0;
		virtual void NotifyListenersForGameOver() const = 0;

		virtual void ApplyMoveUtil(Movement position) = 0;
	};
}