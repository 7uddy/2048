#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Game.h"

using namespace game;

TEST(PieceTest, ConstructorTest)
{
    game::Piece piece{};
    ASSERT_TRUE(piece.GetValue() == 2);
}

TEST(BoardTest, ConstructorTest)
{
    game::Board board{3};

    /*board.PlacePiece(Position{ 0,0 }, std::make_shared<Piece>());
    std::cout << '\n';
    std::cout << board.GetBoard();
    std::cout << '\n';*/
}

TEST(GameTest, MoveTest)
{
    Game game{ 4 };

    //game.SetBoard("2 0 0 \
    //               0 0 0 \
    //               0 0 0");

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.Move(Movement::UP);

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}