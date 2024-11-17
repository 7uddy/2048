#pragma once
#include <memory>
#include "IPiece.h"

namespace game
{
	struct Position 
	{
		int row;
		int column;
	};

	enum class Move
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

	class IBoard
	{
	public:
		virtual void PlacePiece(std::shared_ptr<IPiece> piece) = 0;
		virtual void ErasePiece(Position position) = 0;
		virtual Position GetRandomEmptyPosition() const = 0;
		virtual void MoveBoardToDirection(Move move) = 0;
	};
}