#pragma once


namespace MARIO_LEVEL
{
    constexpr int SMALL = 1;
    constexpr int BIG = 2;
    constexpr int FIRE = 3;
    constexpr int FROG = 4;
}

class PlayerData
{
public:
    int level = MARIO_LEVEL::SMALL;
    int coins = 0;
    int lives = 3;
    int score = 0;
    int returnScene = 1;

    static PlayerData& Get()
    {
        static PlayerData instance;
        return instance;
    }

    void Reset() { level = MARIO_LEVEL::SMALL; coins = 0; lives = 3; score = 0; }
};
