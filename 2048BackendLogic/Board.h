#pragma once
#include <vector>
#include <random>
#include "IBoard.h"

namespace game
{
	class Board : public IBoard
	{
	public:
		void PlacePiece(std::shared_ptr<Piece> piece);
		void ErasePiece(Position position);
		Position GetRandomEmptyPosition() const;
		void MoveBoardToDirection(Move move);
	
	private:
		std::vector<std::shared_ptr<Piece>> m_board;
		unsigned int m_size;
	};
}