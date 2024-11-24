#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Game.h>
#include <ConsoleGameListener.h>

using namespace game;

TEST(PieceTest, ConstructorTest)
{
    game::Piece piece{};
    ASSERT_TRUE(piece.GetValue() == 2);
}

TEST(PieceTest, CombineWithTest)
{
    std::shared_ptr<Piece> p1, p2;
    p1 = std::make_shared<Piece>();
    p2 = std::make_shared<Piece>();

    ASSERT_FALSE(p1->CanCombineWith(p1));
    ASSERT_TRUE(p1->CanCombineWith(p2));

    p2 = p1;
    ASSERT_FALSE(p1->CanCombineWith(p2));
}

TEST(BoardTest, ConstructorTest)
{
    game::Board board{4};/*
    board.PlacePiece(Position{ 0,0 }, std::make_shared<Piece>());
    board.PlacePiece(Position{ 1,0 }, std::make_shared<Piece>());
    board.PlacePiece(Position{ 0,1 }, std::make_shared<Piece>(4));
    board.PlacePiece(Position{ 0,2 }, std::make_shared<Piece>());
    board.PlacePiece(Position{ 0,3 }, std::make_shared<Piece>(4));
    std::cout << board.GetBoard() << std::endl;
    board.Squash(Movement::RIGHT);
    std::cout << board.GetBoard() << std::endl;
    board.Squash(Movement::DOWN);
    std::cout << board.GetBoard() << std::endl;
    board.Squash(Movement::LEFT);
    std::cout << board.GetBoard();*/

}

TEST(GameTest, MoveTest)
{
    Game game{ 3 };

    game.SetBoard("0 0 0\
                   0 2 2\
                   0 2 2");

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.ApplyMove(Movement::RIGHT);

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';
}

TEST(ListenerTest, MovementDoneTest)
{
    /*Game game{ 4 };
    std::shared_ptr<IGameListener> listener{ std::make_shared<ConsoleGameListener>() };
    game.AddListener(listener);

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.ApplyMove(Movement::UP);

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';*/
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}