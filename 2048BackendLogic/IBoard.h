#pragma once
#include <string>
#include "IPiece.h"

namespace game
{
	struct Position 
	{
		unsigned int row;
		unsigned int column;

		Position(unsigned int row, unsigned int column) {
			this->row = row;
			this->column = column;
		}

		static Position FromIndexToPosition(unsigned int boardSize, unsigned int index) 
		{
			unsigned int row{ index / boardSize }; 
			unsigned int column{ index % boardSize };
			return Position{ row, column };
		}

		static unsigned int FromPositionToIndex(unsigned int boardSize, Position position)
		{
			return (position.row * boardSize + position.column);
		}
	};

	enum class Movement
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

	class IBoard
	{
	public:
		virtual void PlacePiece(Position position, std::shared_ptr<IPiece> piece) = 0;
		virtual void ErasePiece(Position position) = 0;
		virtual Position GetRandomEmptyPosition() const = 0;
		virtual void MovePiecesToDirection(Movement move) = 0;

		virtual std::string GetBoard() const = 0;
		virtual void SetBoard(const std::string& board) = 0;
		virtual void PrintBoard() const = 0;
		virtual void ResetBoard() = 0;

	private:
		virtual void MoveRowOrColumnWithData(unsigned int startIndex, unsigned int endIndex, int increaseFactorForStart) = 0;
	};
}