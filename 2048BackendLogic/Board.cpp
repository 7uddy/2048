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
        int currentIndex = (int)startIndex;
        int step = (startIndex < endIndex) ? increaseFactorForStart : -increaseFactorForStart;

        while ((startIndex < endIndex && currentIndex <= (int)endIndex) || (startIndex >= endIndex && currentIndex >= (int)endIndex))
        {
            if (!m_board[currentIndex].get())
            {
                return currentIndex;
            }
            currentIndex += step;
        }

        return INT_MAX;
    };


    int indexOfEarliestEmptyPosition{ GetEarliestEmptyPosition() };
    bool moved{ false };

    if (startIndex < endIndex)
    {
        for (int currentIndex{ (int)startIndex }; currentIndex <= (int)endIndex; currentIndex += increaseFactorForStart)
        {
            //Check if current position is empty.
            if (!m_board[currentIndex].get())
            {
                /*  // This can be completely skipped if using GetEarliestEmptyPosition.
                if (indexOfEarliestEmptyPosition == INT_MAX)
                    indexOfEarliestEmptyPosition = currentIndex;
                */
                continue;
            }

            //Means that there is a piece at currnet position.

            //If there exists an empty position, move the current piece to it.
            if (indexOfEarliestEmptyPosition != INT_MAX)
            {
                m_board[indexOfEarliestEmptyPosition].swap(m_board[currentIndex]);
                
                //Check if there exists a previosu position
                auto previousPosition{ indexOfEarliestEmptyPosition - increaseFactorForStart };
                if (previousPosition < 0 || previousPosition >= m_size * m_size)
                {
                    //Update empty index
                    indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    continue;
                }
                else
                {
                    //Check if they can combine
                    if (m_board[previousPosition] && m_board[previousPosition]->CanCombineWith(m_board[indexOfEarliestEmptyPosition]))
                    {
                        auto newPiece = m_board[previousPosition]->CombinePieces(m_board[indexOfEarliestEmptyPosition]);
                        m_board[indexOfEarliestEmptyPosition].reset();
                        m_board[previousPosition].swap(newPiece);
                    }
                    else
                    {
                        //Update empty index
                        indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    }
                }
            }

            //Check if the current piece can be combined with the previous piece
            auto previousPosition{ currentIndex - increaseFactorForStart };
            if (previousPosition < 0 || previousPosition >= m_size * m_size)
            {
                //No piece was before this one, so no combination.
                continue;
            }

            //Check if the pieces can combine
            if (m_board[previousPosition] && m_board[previousPosition]->CanCombineWith(m_board[currentIndex]))
            {
                auto newPiece = m_board[previousPosition]->CombinePieces(m_board[currentIndex]);
                m_board[currentIndex].reset();
                m_board[previousPosition].swap(newPiece);
                indexOfEarliestEmptyPosition = currentIndex;
            }

        }
    }
    else
    {
        for (int currentIndex{ (int)startIndex }; currentIndex >= (int)endIndex; currentIndex += increaseFactorForStart)
        {
            //Check if current position is empty.
            if (!m_board[currentIndex].get())
            {
                /*  // This can be completely skipped if using GetEarliestEmptyPosition.
                if (indexOfEarliestEmptyPosition == INT_MAX)
                    indexOfEarliestEmptyPosition = currentIndex;
                */
                continue;
            }

            //Means that there is a piece at currnet position.

            //If there exists an empty position, move the current piece to it.
            if (indexOfEarliestEmptyPosition != INT_MAX)
            {
                m_board[indexOfEarliestEmptyPosition].swap(m_board[currentIndex]);

                //Check if there exists a previosu position
                auto previousPosition{ indexOfEarliestEmptyPosition - increaseFactorForStart };
                if (previousPosition < 0 || previousPosition >= m_size * m_size)
                {
                    //Update empty index
                    indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    continue;
                }
                else
                {
                    //Check if they can combine
                    if (m_board[previousPosition] && m_board[previousPosition]->CanCombineWith(m_board[indexOfEarliestEmptyPosition]))
                    {
                        auto newPiece = m_board[previousPosition]->CombinePieces(m_board[indexOfEarliestEmptyPosition]);
                        m_board[indexOfEarliestEmptyPosition].reset();
                        m_board[previousPosition].swap(newPiece);
                    }
                    else
                    {
                        //Update empty index
                        indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    }
                }
            }

            //Check if the current piece can be combined with the previous piece
            auto previousPosition{ currentIndex - increaseFactorForStart };
            if (previousPosition < 0 || previousPosition >= m_size * m_size)
            {
                //No piece was before this one, so no combination.
                continue;
            }

            //Check if the pieces can combine
            if (m_board[previousPosition] && m_board[previousPosition]->CanCombineWith(m_board[currentIndex]))
            {
                auto newPiece = m_board[previousPosition]->CombinePieces(m_board[currentIndex]);
                m_board[currentIndex].reset();
                m_board[previousPosition].swap(newPiece);
                indexOfEarliestEmptyPosition = currentIndex;
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
