#include <iostream>
#include "Piece.h"
#include "Board.h"

static void PieceTests()
{
	using namespace game;

	// Initialization of Piece
	std::shared_ptr<Piece> piece = std::make_shared<Piece>();

	//Check if two pieces can be combined
	std::cout << std::boolalpha << piece->CanCombineWith(std::make_shared<Piece>(2)) << std::endl;

	//Combine two Pieces
	std::shared_ptr<Piece> newPiece = std::dynamic_pointer_cast<Piece>(piece->CombinePieces(std::make_shared<Piece>(4)));

	//Check if a new piece was created
	if (newPiece)
		std::cout << newPiece->GetValue();
	else
		std::cout << "ERROR";
}

static void BoardTests() 
{
	using namespace game;

	//Creation of board
	Board board{ 4 };

	//Place pieces
	board.PlacePiece(Position{ 2, 3 }, std::make_shared<Piece>(4));
	board.PlacePiece(Position{ 2, 0 }, std::make_shared<Piece>(8));
	board.PlacePiece(Position{ 3, 0 }, std::make_shared<Piece>(8));

	//Place piece at an already occupied position. => Combination
	board.PlacePiece(Position{ 2, 0 }, std::make_shared<Piece>(8));

	//Move board
	board.MovePiecesToDirection(Move::UP);

	//Print board
	board.PrintBoard();
}

int main()
{
	BoardTests();
	return 0;
}
