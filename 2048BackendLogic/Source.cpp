#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Game.h"

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
	board.PlacePiece(Position{ 0, 3 }, std::make_shared<Piece>(4));
	board.PlacePiece(Position{ 1, 3 }, std::make_shared<Piece>(8));
	board.PlacePiece(Position{ 2, 3 }, std::make_shared<Piece>(4));
	board.PlacePiece(Position{ 3, 3 }, std::make_shared<Piece>(4));

	//Print board
	board.PrintBoard();
	std::cout << std::endl;

	//Move board
	board.MovePiecesToDirection(Movement::LEFT);
	board.MovePiecesToDirection(Movement::UP);

	//Print board
	board.PrintBoard();
}

static void GameTests()
{
	using namespace game;
	Game game{ 4 };

	std::cout << game.GetBoard() << std::endl << std::endl;

	game.Move(Movement::RIGHT);
	std::cout << game.GetBoard();
}

int main()
{
	GameTests();
	//BoardTests();
	return 0;
}
