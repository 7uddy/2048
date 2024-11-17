#include <iostream>
#include "Piece.h"

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

int main()
{
	PieceTests();
	return 0;
}
