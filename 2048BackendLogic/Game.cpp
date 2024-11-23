#include "Game.h"

game::Game::Game(unsigned int sizeOfBoard) : m_board{ sizeOfBoard }
{
    for (unsigned int i{ 0u }; i < std::min(2u, sizeOfBoard); ++i)
    {
        PlacePieceAtRandomPosition();
    }
}

void game::Game::ApplyMove(Movement direction)
{
    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    bool modificationWasMade{ false };
    for (unsigned int index{ 0 }; index < m_board.GetBoardSize(); ++index)
        modificationWasMade = modificationWasMade || m_board.SquashColumn(index);

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    if (modificationWasMade)
        PlacePieceAtRandomPosition();

    NotifyListenersForMoveDone();
    
    if (IsGameOver())
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
    if (m_board.IsBoardFull())
        return;

    Position position{ m_board.GetRandomEmptyPosition() };
    while (m_board.GetPieceAtPosition(position))
    {
        position = m_board.GetRandomEmptyPosition();
    }
    m_board.PlacePiece(position, std::make_shared<Piece>());
}

bool game::Game::IsGameOver()
{

    std::string currentBoard{ m_board.GetBoard() };

    ApplyMoveUtil(Movement::UP);
    ApplyMoveUtil(Movement::DOWN);
    ApplyMoveUtil(Movement::LEFT);
    ApplyMoveUtil(Movement::RIGHT);

    std::string newBoard{ m_board.GetBoard() };

    if (currentBoard != newBoard)
    {
        m_board.SetBoard(currentBoard);
        return false;
    }
    return true;
}

void game::Game::ApplyMoveUtil(Movement direction)
{
    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    bool modificationWasMade{ false };
    for (unsigned int index{ 0 }; index < m_board.GetBoardSize(); ++index)
    {
        modificationWasMade = modificationWasMade || m_board.SquashColumn(index);
        if (modificationWasMade)
            return;
    }

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();
}