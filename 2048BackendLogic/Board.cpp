#include "Board.h"
#include <iostream>

game::Board::Board(unsigned int size) : m_size{ size }, m_numberOfPiecesOnBoard{ 0u }
{
    m_board.resize(m_size * m_size);
}

void game::Board::PlacePiece(Position position, std::shared_ptr<IPiece> piece)
{
    unsigned int indexInBoard{ Position::FromPositionToIndex(m_size, position) };

    //Check if input position is valid
    if (indexInBoard > m_board.size())
    {
        std::cout << "Invalid position(" << position.row << ", " << position.column << ")" << std::endl;
        return;
    }

    if (m_board[indexInBoard].get())
    {
        //std::cout << "There is already a piece at (" << position.row << ", " << position.column << ")" << std::endl;
        
        //there already is a piece there, check if they can be combined
        if (m_board[indexInBoard]->CanCombineWith(piece))
        {
            std::cout << "Placed " << m_board[indexInBoard]->GetValue() << " at position (" << position.row << ", " << position.column << ")";
            m_board[indexInBoard] = std::dynamic_pointer_cast<Piece>(m_board[indexInBoard]->CombinePieces(piece));
            std::cout << " => Placed combined " << m_board[indexInBoard]->GetValue() << " at position (" << position.row << ", " << position.column << ")" << std::endl;
        }
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
    unsigned int startIndex{}, endIndex{};
    int increaseFactorForStart{}, increaseFactorForEnd{};


    switch (move)
    {
    case Move::DOWN:
        startIndex = m_size * (m_size - 1);
        endIndex = 0u;
        increaseFactorForStart = -(int)m_size;
        increaseFactorForEnd = 1u;

        break;
    case Move::UP:
        startIndex = 0u;
        endIndex = m_size * (m_size - 1);
        increaseFactorForStart = m_size;
        increaseFactorForEnd = 1u;

        break;
    case Move::RIGHT:
        startIndex = m_size - 1u;
        endIndex = 0u;
        increaseFactorForStart = -1;
        increaseFactorForEnd = m_size;

        break;
    case Move::LEFT:
        startIndex = 0u;
        endIndex = m_size - 1u;
        increaseFactorForStart = 1;
        increaseFactorForEnd = m_size;

        break;
    default:
        std::cout << "Invalid move.";

        break;
    }

    //Process each line or column based on direction
    for (int index{ 0 }; index < (int)m_size; index++)
    {
        MoveRowOrColumnWithData(startIndex, endIndex, increaseFactorForStart);
        
        //Move to the next line
        if (move == Move::LEFT || move == Move::RIGHT)
        {
            startIndex += m_size;
            endIndex += m_size;
        }
        //Move to the next column
        else
        {
            startIndex += 1u;
            endIndex += 1u;
        }
    }

}

void game::Board::MoveRowOrColumnWithData(unsigned int startIndex, unsigned int endIndex, int increaseFactorForStart)
{
    auto GetEarliestEmptyPosition = [&]() {
        if (startIndex < endIndex)
        {
            for (int currentIndex{ (int)startIndex }; currentIndex <= (int)endIndex; currentIndex += increaseFactorForStart)
            {
                if (!m_board[currentIndex].get())
                {
                    return currentIndex;
                }

            }
        }
        else
        {
            for (int currentIndex{ (int)startIndex }; currentIndex >= (int)endIndex; currentIndex += increaseFactorForStart)
            {
                if (!m_board[currentIndex].get())
                {
                    return currentIndex;
                }

            }
        }
    };

    unsigned int indexOfEarliestEmptyPosition{ INT_MAX }, indexOfSecondEarliestEmptyPosition{ INT_MAX };
    bool moved{ false };

    if (startIndex < endIndex)
    {
        for (int currentIndex{ (int)startIndex }; currentIndex <= (int)endIndex; currentIndex += increaseFactorForStart)
        {
            //Check if current position has a piece and update emptyPosition if there wasn't found one already
            if (indexOfEarliestEmptyPosition == INT_MAX && !m_board[currentIndex].get())
            {
                indexOfEarliestEmptyPosition = currentIndex;
            }

            if (!m_board[currentIndex].get())
                continue;

            //If there exists an empty position, move the current piece to it
            moved = false;
            if (indexOfEarliestEmptyPosition != currentIndex && indexOfEarliestEmptyPosition != INT_MAX)
            {
                m_board[indexOfEarliestEmptyPosition].swap(m_board[currentIndex]);
                moved = true;
                indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                //indexOfEarliestEmptyPosition = INT_MAX;
            }

        }
    }
    else
    {
        for (int currentIndex{ (int)startIndex }; currentIndex >= (int)endIndex; currentIndex += increaseFactorForStart)
        {
            //Check if current position has a piece and update emptyPosition if there wasn't found one already
            if (indexOfEarliestEmptyPosition == INT_MAX && !m_board[currentIndex].get())
            {
                indexOfEarliestEmptyPosition = currentIndex;
            }

            if (!m_board[currentIndex].get())
                continue;

            //If there exists an empty position, move the current piece to it
            moved = false;
            if (indexOfEarliestEmptyPosition != currentIndex && indexOfEarliestEmptyPosition != INT_MAX)
            {
                m_board[indexOfEarliestEmptyPosition].swap(m_board[currentIndex]);
                moved = true;
                indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                //indexOfEarliestEmptyPosition = INT_MAX;
            }

        }
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
