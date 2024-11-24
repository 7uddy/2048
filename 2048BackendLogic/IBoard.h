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
	};

	struct MoveResult
	{
		unsigned int scoreGained;
		bool modificationWasMade;

		MoveResult(unsigned int scoreGained, bool modificationWasMade) {
			this->scoreGained = scoreGained;
			this->modificationWasMade = modificationWasMade;
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
		virtual std::shared_ptr<IPiece> GetPieceAtPosition(Position position) const = 0;
		virtual void PlacePiece(Position position, std::shared_ptr<IPiece> piece) = 0;
		virtual void ErasePiece(Position position) = 0;
		virtual void SwapPiecesAtPositions(Position position1, Position position2) = 0;

		virtual Position GetRandomEmptyPosition() const = 0;

		virtual std::string GetBoard() const = 0;
		virtual void SetBoard(const std::string& board) = 0;
		virtual void ResetBoard() = 0;

		virtual bool IsBoardFull() const = 0;
		virtual unsigned int GetBoardSize() const = 0;

		virtual MoveResult SquashColumn(unsigned int columIndex) = 0;
		virtual void FlipVertically() = 0;
		virtual void FlipDiagonally() = 0;

		virtual bool IsPositionValid(Position position) const = 0;
	};
}