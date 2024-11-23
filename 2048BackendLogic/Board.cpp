#include "Board.h"
#include <iostream>

using namespace game;

game::Board::Board(unsigned int size) : m_size{ size }, m_numberOfPiecesOnBoard{ 0u }
{
    for (unsigned int indexLine{ 0u }; indexLine < m_size; ++indexLine)
    {
        std::vector<std::shared_ptr<IPiece>> newLine{ m_size };
        m_board.emplace_back(std::move(newLine));
    }
}

void game::Board::PlacePiece(Position position, std::shared_ptr<IPiece> piece)
{
    if (!IsPositionValid(position))
        return;

    if (!m_board[position.row][position.column])
    {
        m_numberOfPiecesOnBoard++;
    }
    m_board[position.row][position.column] = std::move(piece);
}

void game::Board::ErasePiece(Position position)
{
    if (!IsPositionValid(position))
        return;

    if (m_board[position.row][position.column])
    {
        m_numberOfPiecesOnBoard--;
    }
    m_board[position.row][position.column].reset();
}

game::Position game::Board::GetRandomEmptyPosition() const
{
    Position pos{ INT_MAX, INT_MAX };
    if (IsBoardFull())
        return pos;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, m_size - 1);
    do 
    {
        pos.row = distrib(gen);
        pos.column = distrib(gen);
    } while (m_board[pos.row][pos.column]);
    return pos;
}

void game::Board::ResetBoard()
{
    for(auto & line : m_board)
    {
        for(auto& piece : line)
            piece.reset();
    }
    m_numberOfPiecesOnBoard = 0u;
}

bool game::Board::IsBoardFull() const
{
    return m_numberOfPiecesOnBoard == (m_size * m_size);
}

unsigned int game::Board::GetBoardSize() const
{
    return m_size;
}

bool game::Board::SquashColumn(unsigned int columnIndex)
{
    bool modificationWasMade{ false };
    for (int destinationIndex{ (int)m_size - 1 }; destinationIndex >= 0; --destinationIndex)
    {
        int sourceIndex{ destinationIndex - 1 };
        while (sourceIndex >= 0 && !m_board[sourceIndex][columnIndex])
            --sourceIndex;

        if (sourceIndex < 0)
            break;

        if (m_board[destinationIndex][columnIndex] && m_board[sourceIndex][columnIndex] && m_board[destinationIndex][columnIndex]->CanCombineWith(m_board[sourceIndex][columnIndex]))
        {
            PlacePiece(Position{ (unsigned int)destinationIndex, columnIndex }, m_board[sourceIndex][columnIndex]->CombinePieces(m_board[destinationIndex][columnIndex]));
            ErasePiece(Position{ (unsigned int)sourceIndex, columnIndex });
            modificationWasMade = true;
        }

        if (!m_board[destinationIndex][columnIndex])
        {
            SwapPiecesAtPositions(Position{ (unsigned int)destinationIndex, columnIndex }, Position{ (unsigned int)sourceIndex, columnIndex });
            ++destinationIndex;
            modificationWasMade = true;
        }
    }
    return modificationWasMade;
}

void game::Board::FlipVertically()
{
    unsigned int indexLine{ 0 }, increaseFactor{ 0 };
    for (; indexLine < m_size; ++indexLine)
    {
        for (increaseFactor = 0; increaseFactor < m_size / 2; ++increaseFactor)
        {
            int newIndex = (int)m_size - 1 - increaseFactor;
            m_board[increaseFactor][indexLine].swap(m_board[newIndex][indexLine]);
        }
    }
}

void game::Board::FlipDiagonally()
{
    unsigned rowIndex{ 0 }, columnIndex{};
    for (; rowIndex < m_size; ++rowIndex)
    {
        for (columnIndex = rowIndex + 1; columnIndex < m_size; ++columnIndex)
        {
            m_board[rowIndex][columnIndex].swap(m_board[columnIndex][rowIndex]);
        }
    }
}

bool game::Board::IsPositionValid(Position position) const
{
    if (position.row < 0 || position.row >= m_size)
        return false;
    if (position.column < 0 || position.column >= m_size)
        return false;

    return true;
}

void game::Board::SwapPiecesAtPositions(Position position1, Position position2)
{
    if (!IsPositionValid(position1) || !IsPositionValid(position2))
        return;

    m_board[position1.row][position1.column].swap(m_board[position2.row][position2.column]);
}

std::shared_ptr<IPiece> game::Board::GetPieceAtPosition(Position position) const
{
    if (!IsPositionValid(position))
        return nullptr;

    return m_board[position.row][position.column];
}

std::string game::Board::GetBoard() const
{
    std::ostringstream boardStream;

    for (size_t indexLine{ 0u }; indexLine < m_board.size(); ++indexLine)
    {
        for (size_t indexColumn{ 0u }; indexColumn < m_board.size(); ++indexColumn)
        {
            if (m_board[indexLine][indexColumn])
            {
                boardStream << m_board[indexLine][indexColumn]->GetValue() << " ";
            }
            else
            {
                boardStream << "0 ";
            }
        }
        boardStream << std::endl;
    }

    return boardStream.str();
}

void game::Board::SetBoard(const std::string& board)
{
    ResetBoard();
    std::istringstream stream(board);
    unsigned int currentRowIndex{ 0u }, currentColumnIndex{ 0u };
    int number{ 0 };
    try
    {
        while (stream >> number && currentRowIndex < m_size)
        {
            if (number == 0)
            {
                currentColumnIndex++;
                if (currentColumnIndex == m_size)
                {
                    currentRowIndex++;
                    currentColumnIndex = 0u;
                }
                continue;
            }
            PlacePiece(Position{ currentRowIndex, currentColumnIndex++ }, std::make_shared<Piece>(number));
        }
    }
    catch (...)
    {
        ResetBoard();
    }
}
