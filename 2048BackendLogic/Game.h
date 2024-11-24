#pragma once
#include <iostream>

#include "IGame.h"
#include "Board.h"

namespace game
{
	class Game : public IGame
	{
	public:
		Game(unsigned int sizeOfBoard);

		void ApplyMove(Movement move) override;

		bool IsGameOver() override;

		void ResetGame() override;

		unsigned int GetScore() const override;

		void SetBoard(const std::string& board) override;
		std::string GetBoard() const override;

		void AddListener(IGameListenerPtr observer) override;
		void RemoveListener(IGameListener* observer) override;

	private:
		void PlacePieceAtRandomPosition() override;
		bool ApplyMoveUtil(Movement position) override;

		void NotifyListenersForMoveDone() const override;
		void NotifyListenersForGameOver() const override;

	private:
		Board m_board;
		std::vector<IGameListenerWeakPtr> m_observers;
		unsigned int m_score;
	};
}
