#include "Board.h"

void game::Board::PlacePiece(std::shared_ptr<Piece> piece)
{
}

void game::Board::ErasePiece(Position position)
{
}

game::Position game::Board::GetRandomEmptyPosition() const
{
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distrib(1, m_size*m_size);
    while(m_board[distrib(gen)]->GetValue() )
    return Position();
}

void game::Board::MoveBoardToDirection(Move move)
{
}
