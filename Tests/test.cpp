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
    game::Board board{3};
}

TEST(GameTest, MoveTest)
{
    Game game{ 3 };

    //game.SetBoard("2 0 0 0 \
    //               0 0 0 0\
    //               0 0 0 0\
    //               0 0 0 0");

    /*std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.Move(Movement::UP);

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.Move(Movement::UP); 
    
    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.Move(Movement::UP); 
    
    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.Move(Movement::UP);*/



}

TEST(ListenerTest, MovementDoneTest)
{
    /*Game game{ 4 };
    std::shared_ptr<IGameListener> listener{ std::make_shared<ConsoleGameListener>() };
    game.AddListener(listener);

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';

    game.Move(Movement::UP);

    std::cout << '\n';
    std::cout << game.GetBoard();
    std::cout << '\n';*/
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}