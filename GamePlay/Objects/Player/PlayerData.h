#pragma once

// Trạng thái người chơi, giữ xuyên suốt các màn chơi (không bị xoá khi đổi scene).
// Meyers singleton: dùng static cục bộ trong hàm -> sống suốt vòng đời game,
// không cần .cpp và không phải thêm file vào .vcxproj.

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
    int level = MARIO_LEVEL::FIRE;
    int coins = 0;
    int lives = 3;
    int returnScene = 1;   // level to resume after the death screen (default: world 1-1)

    static PlayerData& Get()
    {
        static PlayerData instance;
        return instance;
    }

    // Gọi khi muốn chơi lại từ đầu (vd: hết mạng / new game)
    void Reset() { level = MARIO_LEVEL::FIRE; coins = 0; lives = 3; }
};
