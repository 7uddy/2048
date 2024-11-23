#include "Game.h"

game::Game::Game(unsigned int sizeOfBoard) : m_board{ sizeOfBoard }
{
    for (int i = 0; i < 2; i++)
    {
        PlacePieceAtRandomPosition();
    }
}

void game::Game::Move(Movement direction)
{
    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    for (unsigned int index{ 0 }; index < m_board.GetBoardSize(); ++index)
        m_board.SquashColumn(index);

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    PlacePieceAtRandomPosition();

    //TO DO - TO BE IMPLEMENTED
    if (/*CanStillMove()*/ true)
    {
        NotifyListenersForMoveDone();
    }
    else
    {
        NotifyListenersForGameOver();
    }

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

void game::Game::NotifyListenersForMoveDone() const
{
    for (auto& listener : m_observers)
    {
        if (auto sp = listener.lock())
        {
            sp->OnMoveDone();
        }

    }
}

void game::Game::NotifyListenersForGameOver() const
{
    for (auto& listener : m_observers)
    {
        if (auto sp = listener.lock())
        {
            sp->OnGameOver();
        }
    }
}

void game::Game::PlacePieceAtRandomPosition()
{
    Position position{ m_board.GetRandomEmptyPosition() };
    while (m_board.GetPieceAtPosition(position))
    {
        position = m_board.GetRandomEmptyPosition();
    }
    m_board.PlacePiece(position, std::make_shared<Piece>());
}
