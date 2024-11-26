#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Game.h>
#include <ConsoleGameListener.h>

using namespace game;

TEST(PieceTest, InstantiationWithDefaultValue)
{
    Piece piece{};
    EXPECT_EQ(piece.GetValue(), 2) << "Default value for a piece should be 2";
}

TEST(PieceTest, InstantiationWithCustomValue) {
    Piece piece(10);
    EXPECT_EQ(piece.GetValue(), 10) << "Value for a piece should match the custom value provided";
}

TEST(PieceTest, SetValue) {
    Piece piece;
    piece.SetValue(20);
    EXPECT_EQ(piece.GetValue(), 20) << "Piece's value should be updated to 20";
}


TEST(PieceTest, CombineWithTest)
{
    std::shared_ptr<IPiece> p1, p2;
    p1 = std::make_shared<Piece>();
    p2 = std::make_shared<Piece>();

    ASSERT_FALSE(p1->CanCombineWith(p1));
    ASSERT_TRUE(p1->CanCombineWith(p2));

    p2 = p1;
    ASSERT_FALSE(p1->CanCombineWith(p2));
}

TEST(PieceTest, CombinePieces) {
    Piece piece1(4);
    Piece piece2(4);
    ASSERT_TRUE(piece1.CanCombineWith(std::make_shared<Piece>(piece2))) << "Should be able to combine the pieces with equal values";
    auto combinedPiece = piece1.CombinePieces(std::make_shared<Piece>(piece2));
    ASSERT_NE(combinedPiece, nullptr);
    EXPECT_EQ(combinedPiece->GetValue(), 8);
}



TEST(BoardTest, InstantiationTest)
{
    game::Board board{4};
    ASSERT_EQ(board.GetBoardSize(), 4);
    ASSERT_EQ(board.GetNumberOfPiecesOnBoard(), 0);
}

TEST(BoardTest, PlacePieceTest)
{
    game::Board board(4);
    game::Position pos{ 0, 0 };
    auto piece = std::make_shared<game::Piece>(2);

    board.PlacePiece(pos, piece);
    ASSERT_EQ(board.GetNumberOfPiecesOnBoard(), 1);
    ASSERT_NE(board.GetPieceAtPosition(pos), nullptr);
    ASSERT_EQ(board.GetPieceAtPosition(pos)->GetValue(), 2);
}

TEST(BoardTest, PositionValidityTest)
{
    game::Board board(4);
    game::Position validPos{ 2, 2 };
    game::Position invalidPos{ 5, 5 };

    ASSERT_TRUE(board.IsPositionValid(validPos));  
    ASSERT_FALSE(board.IsPositionValid(invalidPos));
}

TEST(BoardTest, ErasePieceTest)
{
    game::Board board(4);
    game::Position pos{ 0, 0 };
    auto piece = std::make_shared<game::Piece>(2);

    board.PlacePiece(pos, piece);
    ASSERT_EQ(board.GetNumberOfPiecesOnBoard(), 1);
    board.ErasePiece(pos);
    ASSERT_EQ(board.GetNumberOfPiecesOnBoard(), 0);  
    ASSERT_EQ(board.GetPieceAtPosition(pos), nullptr);
}

//TEST(BoardTest, GetRandomEmptyPositionTest)
//{
//    game::Board board(4);
//    auto emptyPos = board.GetRandomEmptyPosition();
//    ASSERT_TRUE(board.IsPositionValid(emptyPos));
//    ASSERT_EQ(board.GetPieceAtPosition(emptyPos), nullptr);
//}

TEST(BoardTest, IsBoardFullTest)
{
    game::Board board(4);
    ASSERT_FALSE(board.IsBoardFull());

    for (unsigned int row = 0; row < 4; ++row)
    {
        for (unsigned int col = 0; col < 4; ++col)
        {
            board.PlacePiece(game::Position{ row, col }, std::make_shared<game::Piece>(2));
        }
    }
    ASSERT_TRUE(board.IsBoardFull());
    game::Board anotherBoard(4);
    anotherBoard.SetBoard(board.GetBoard());
    ASSERT_TRUE(anotherBoard.IsBoardFull())<<"Set Board is not working properly";
}

TEST(BoardTest, ResetBoardTest)
{
    game::Board board(4);
    game::Position pos{ 0, 0 };
    auto piece = std::make_shared<game::Piece>(2);

    board.PlacePiece(pos, piece);
    ASSERT_EQ(board.GetNumberOfPiecesOnBoard(), 1); 

    board.ResetBoard();
    ASSERT_EQ(board.GetNumberOfPiecesOnBoard(), 0);
    ASSERT_EQ(board.GetPieceAtPosition(pos), nullptr);
}

TEST(BoardTest, SquashColumnTest)
{
    game::Board board(4);
    game::Position pos1{ 0, 0 };
    game::Position pos2{ 1, 0 };
    game::Position pos3{ 2, 0 };
    auto piece1 = std::make_shared<game::Piece>(2);
    auto piece2 = std::make_shared<game::Piece>(2);
    auto piece3 = std::make_shared<game::Piece>(2);

    board.PlacePiece(pos1, piece1);
    board.PlacePiece(pos2, piece2);
    board.PlacePiece(pos3, piece3);

    game::MoveResult result = board.SquashColumn(0);
    ASSERT_TRUE(result.modificationWasMade);
    game::Position expectedPos1{ 3,0 };
    game::Position expectedPos2{ 2,0 };
    ASSERT_EQ(board.GetPieceAtPosition(expectedPos1)->GetValue(), 4);
    ASSERT_EQ(board.GetPieceAtPosition(expectedPos2)->GetValue(), 2);
}

TEST(BoardTest, FlipVerticallyTest)
{
    game::Board board(4);
    game::Position pos1{ 0, 0 };
    game::Position pos2{ 3, 0 };
    auto piece1 = std::make_shared<game::Piece>(2);
    auto piece2 = std::make_shared<game::Piece>(4);

    board.PlacePiece(pos1, piece1);
    board.PlacePiece(pos2, piece2);
    board.FlipVertically();

    ASSERT_EQ(board.GetPieceAtPosition(pos2), piece1)<<"Vertical flip error";
    ASSERT_EQ(board.GetPieceAtPosition(pos1), piece2) << "Vertical flip error";
}

TEST(BoardTest, FlipDiagonallyTest)
{
    Board board(4);

    board.PlacePiece(Position{ 0, 0 }, std::make_shared<Piece>(1));
    board.PlacePiece(Position{ 0, 1 }, std::make_shared<Piece>(2));
    board.PlacePiece(Position{ 0, 2 }, std::make_shared<Piece>(3));
    board.PlacePiece(Position{ 0, 3 }, std::make_shared<Piece>(4));

    board.PlacePiece(Position{ 1, 0 }, std::make_shared<Piece>(5));
    board.PlacePiece(Position{ 1, 1 }, std::make_shared<Piece>(6));
    board.PlacePiece(Position{ 1, 2 }, std::make_shared<Piece>(7));
    board.PlacePiece(Position{ 1, 3 }, std::make_shared<Piece>(8));

    board.PlacePiece(Position{ 2, 0 }, std::make_shared<Piece>(9));
    board.PlacePiece(Position{ 2, 1 }, std::make_shared<Piece>(10));
    board.PlacePiece(Position{ 2, 2 }, std::make_shared<Piece>(11));
    board.PlacePiece(Position{ 2, 3 }, std::make_shared<Piece>(12));

    board.PlacePiece(Position{ 3, 0 }, std::make_shared<Piece>(13));
    board.PlacePiece(Position{ 3, 1 }, std::make_shared<Piece>(14));
    board.PlacePiece(Position{ 3, 2 }, std::make_shared<Piece>(15));
    board.PlacePiece(Position{ 3, 3 }, std::make_shared<Piece>(16));

    std::string initialBoard = board.GetBoard();
    std::string expectedInitialBoard =
        "1 2 3 4 \n"
        "5 6 7 8 \n"
        "9 10 11 12 \n"
        "13 14 15 16 \n";
    EXPECT_EQ(initialBoard, expectedInitialBoard)<<"Get Board error";

    board.FlipDiagonally();

    std::string flippedBoard = board.GetBoard();
    std::string expectedFlippedBoard =
        "1 5 9 13 \n"
        "2 6 10 14 \n"
        "3 7 11 15 \n"
        "4 8 12 16 \n";

    EXPECT_EQ(flippedBoard, expectedFlippedBoard);
}







//TEST(GameTest, MoveTest)
//{
//    Game game{ 3 };
//
//    game.SetBoard("0 0 0\
//                   0 2 2\
//                   0 2 2");
//
//    std::cout << '\n';
//    std::cout << game.GetBoard();
//    std::cout << '\n';
//
//    game.ApplyMove(Movement::RIGHT);
//
//    std::cout << '\n';
//    std::cout << game.GetBoard();
//    std::cout << '\n';
//}
//
//TEST(ListenerTest, MovementDoneTest)
//{
//    /*Game game{ 4 };
//    std::shared_ptr<IGameListener> listener{ std::make_shared<ConsoleGameListener>() };
//    game.AddListener(listener);
//
//    std::cout << '\n';
//    std::cout << game.GetBoard();
//    std::cout << '\n';
//
//    game.ApplyMove(Movement::UP);
//
//    std::cout << '\n';
//    std::cout << game.GetBoard();
//    std::cout << '\n';*/
//}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}