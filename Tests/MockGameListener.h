#pragma once

#include <gmock/gmock.h>
#include "IGameListener.h"

namespace game
{
    class MockGameListener : public IGameListener
    {
    public:
        MOCK_METHOD(void, OnMoveDone, (), (override));
        MOCK_METHOD(void, OnGameOver, (), (override));
        MOCK_METHOD(void, OnGameReset, (), (override));
        MOCK_METHOD(void, OnEnableUndoMove, (bool isEnabled), (override));
        MOCK_METHOD(void, OnEnableSwapTileMove, (bool isEnabled), (override));
    };
}
