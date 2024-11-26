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
    };
}
