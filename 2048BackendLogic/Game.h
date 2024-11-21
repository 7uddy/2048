#pragma once
#include "IGame.h"
#include "Board.h"

namespace game
{
	class Game : public IGame
	{
	public:
		Game(unsigned int sizeOfBoard);
		void Move(Movement move) override;
		void SetBoard(const std::string& board) override;
		std::string GetBoard() const override;

		void AddListener(IGameListenerPtr observer) override;
		void RemoveListener(IGameListener* observer) override;
	private:
		Board m_board;
		std::vector<IGameListenerWeakPtr> m_observers;
	};
}