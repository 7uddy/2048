#include "Game.h"

game::Game::Game(unsigned int sizeOfBoard) : m_board{sizeOfBoard}
{
	auto position = m_board.GetRandomEmptyPosition();
	m_board.PlacePiece(position, std::make_shared<Piece>());
}

void game::Game::Move(Movement direction)
{
	m_board.MovePiecesToDirection(direction);
}

void game::Game::SetBoard(const std::string& board)
{
	m_board.SetBoard(board);
}

std::string game::Game::GetBoard() const
{
	return m_board.GetBoard();
}

void game::Game::AddListener(IGameListenerPtr observer)
{
    m_observers.emplace_back(observer);
}

void game::Game::RemoveListener(IGameListener* observer)
{
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		if (auto sp = it->lock())
		{
			if (sp.get() == observer)
				it = m_observers.erase(it);
			else
				it++;
		}
		else
			it = m_observers.erase(it);
	}
}
