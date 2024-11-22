#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Piece.h"

TEST(PieceTest, ConstructorTest)
{
    game::Piece piece{};
    ASSERT_TRUE(piece.GetValue() == 3);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}