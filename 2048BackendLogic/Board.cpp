#include "Board.h"
#include <iostream>

game::Board::Board(unsigned int size) : m_size{ size }, m_numberOfPiecesOnBoard{ 0u }
{
    m_board.resize(m_size * m_size);
}

void game::Board::PlacePiece(Position position, std::unique_ptr<IPiece> piece)
{
    unsigned int indexInBoard{ Position::FromPositionToIndex(m_size, position) };
    if (indexInBoard > m_board.size())
    {
        std::cout << "Invalid position(" << position.row << ", " << position.column << ")" << std::endl;
        return;
    }

    if (m_board[indexInBoard].get())
    {
        //TO DO -> trebuie implementata piesa

        std::cout << "There is already a piece at (" << position.row << ", " << position.column << ")" << std::endl;
        //there already is a piece there, check if they can be combined
    }
    else
    {
        std::cout << "Placed " << piece->GetValue() << " at position (" << position.row << ", " << position.column << ")" << std::endl;
        m_board[indexInBoard] = std::move(piece);
        m_numberOfPiecesOnBoard++;
    }
}

void game::Board::ErasePiece(Position position)
{
    m_board[Position::FromPositionToIndex(m_size, position)].reset();
}

game::Position game::Board::GetRandomEmptyPosition() const
{
    //TO DO -> nu stiu ce sa returnez sau ce sa fac cand nu mai exista pozitii goale

    if (m_numberOfPiecesOnBoard == m_size * m_size)
        return Position(INT_MAX, INT_MAX);
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

void game::Board::MovePiecesToDirection(Move move)
{
    //TO DO -> trebuie implementata functia care traverseaza board-ul

    switch (move)
    {
    case Move::UP:

        break;
    case Move::DOWN:

        break;
    case Move::LEFT:

        break;
    case Move::RIGHT:

        break;
    default:
        std::cout << "Invalid move.";

        break;
    }
}

void game::Board::ResetBoard()
{
    for(auto & position : m_board)
    {
        position.reset();
    }
    m_numberOfPiecesOnBoard = 0u;
}

std::string game::Board::GetBoard() const
{
    std::ostringstream boardStream;

    for (size_t index{ 0u }; index < m_board.size(); ++index)
    {
        if (index % m_size == 0) 
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

void game::Board::PrintBoard() const
{
    std::cout << GetBoard();
}
