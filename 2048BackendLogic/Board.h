#pragma once
#include <vector>
#include <random>
#include <sstream>
#include "IBoard.h"
#include "Piece.h"

namespace game
{
	class Board : public IBoard
	{
	public:
		Board(unsigned int size);

		//Existence of pieces
		std::shared_ptr<IPiece> GetPieceAtPosition(Position position) const override;
		void PlacePiece(Position position, std::shared_ptr<IPiece> piece) override;
		void ErasePiece(Position position) override;
		void SwapPiecesAtPositions(Position position1, Position position2) override;

		//Get empty position of board
		Position GetRandomEmptyPosition() const override;

		//Functions related to board state
		std::string GetBoard() const override;
		void SetBoard(const std::string& board) override;
		void ResetBoard() override;

		//Functions related to board data
		bool IsBoardFull() const override;
		unsigned int GetBoardSize() const override;

		//Functions related to movement
		MoveResult SquashColumn(unsigned int columIndex) override;
		void FlipVertically() override;
		void FlipDiagonally() override;

		//Function related to position on board
		bool IsPositionValid(Position position) const override;

	private:
		std::vector<std::vector<std::shared_ptr<IPiece>>> m_board;
		unsigned int m_size;
		unsigned int m_numberOfPiecesOnBoard;

	};
}