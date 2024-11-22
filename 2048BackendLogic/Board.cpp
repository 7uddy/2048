#include "Board.h"
#include <iostream>

using namespace game;

game::Board::Board(unsigned int size) : m_size{ size }, m_numberOfPiecesOnBoard{ 0u }
{
    m_board.resize(m_size * m_size);
}

void game::Board::PlacePiece(Position position, std::shared_ptr<IPiece> piece)
{
    const auto& positionInVector{ Position::FromPositionToIndex(m_size, position) };
    if (!m_board[positionInVector])
    {
        m_numberOfPiecesOnBoard++;
    }
    m_board[positionInVector] = std::move(piece);
}

void game::Board::ErasePiece(Position position)
{
    const auto& positionInVector{ Position::FromPositionToIndex(m_size, position) };
    if (m_board[positionInVector])
    {
        m_numberOfPiecesOnBoard--;
    }
    m_board[positionInVector].reset();
}

game::Position game::Board::GetRandomEmptyPosition() const
{
    if (m_numberOfPiecesOnBoard == m_size * m_size)
        return Position(NULL, NULL);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, m_size * m_size - 1);
    int index{ 0 };
    do 
    {
        index = distrib(gen);
    } while (m_board[index].get());
    return Position::FromIndexToPosition(m_size, index);
}

void game::Board::ResetBoard()
{
    for(auto & position : m_board)
    {
        position.reset();
    }
    m_numberOfPiecesOnBoard = 0u;
}

int game::Board::GetNumberOfPiecesOnBoard() const
{
    return m_numberOfPiecesOnBoard;
}

int game::Board::GetBoardSize() const
{
    return m_size;
}

void game::Board::SwapPiecesAtPositions(Position position1, Position position2)
{
    m_board[Position::FromPositionToIndex(m_size, position1)].swap(m_board[Position::FromPositionToIndex(m_size, position2)]);
}

std::shared_ptr<IPiece> game::Board::GetPieceAtPosition(Position position) const
{
    return m_board[Position::FromPositionToIndex(m_size, position)];
}

std::string game::Board::GetBoard() const
{
    std::ostringstream boardStream;

    for (size_t index{ 0u }; index < m_board.size(); ++index)
    {
        if (index != 0 && index % m_size == 0)
        {
            boardStream << std::endl;
        }

        if (m_board[index].get())
        {
            boardStream << m_board[index].get()->GetValue() << " ";
        }
        else
        {
            boardStream << "0 ";
        }
    }

    return boardStream.str();
}

void game::Board::SetBoard(const std::string& board)
{
    ResetBoard();
    std::istringstream stream(board);
    unsigned int currentIndex{ 0u };
    unsigned int number{ 0u };

    while (stream >> number)
    {
        if (number == 0)
        {
            currentIndex++;
            continue;
        }
        PlacePiece(Position::FromIndexToPosition(m_size, currentIndex++), std::make_unique<Piece>(number));
    }
}
