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
		void PlacePiece(Position position, std::shared_ptr<IPiece> piece) override;
		void ErasePiece(Position position) override;
		Position GetRandomEmptyPosition() const override;
		
		
		//Movement of board
		void MovePiecesToDirection(Movement move) override;


		//Functions related to board state
		std::string GetBoard() const override;
		void SetBoard(const std::string& board) override;
		void PrintBoard() const override;
		void ResetBoard() override;

	private:
		void MoveRowOrColumnWithData(unsigned int startIndex, unsigned int endIndex, int increaseFactorForStart) override;

	private:
		std::vector<std::shared_ptr<IPiece>> m_board;
		unsigned int m_size;
		unsigned int m_numberOfPiecesOnBoard;

	};
}