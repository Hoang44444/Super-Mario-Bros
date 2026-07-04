#pragma once
#include "PlayerData.h"

// Score values cho các hành động trong game
namespace SCORE_VALUES
{
	constexpr int GOOMBA_STOMP = 100;        // Đạp Goomba
	constexpr int KOOPA_STOMP = 100;         // Đạp Koopa
	constexpr int COIN = 200;                // Nhặt xu
	constexpr int BRICK_DESTROYED = 50;      // Phá gạch
	constexpr int SUPER_MUSHROOM = 1000;     // Nhặt nấm lớn
	constexpr int FIRE_FLOWER = 1000;        // Nhặt hoa lửa
	constexpr int STARMAN = 1000;             // Nhặt ngôi sao
	constexpr int SHELL_KICK = 100;          // Đạp vỏ rùa
	constexpr int FIREBALL_KILL = 100;       // Giết bằng đạn lửa
	constexpr int STAR_KILL = 100;            // Giết khi vô hình
	constexpr int QUESTION_BLOCK_COIN = 200; // Xu từ question block
	constexpr int LEVEL_COMPLETION = 0;       // Hoàn thành level (chưa implement)
}

// Combo scores - điểm thưởng khi giết liên tiếp enemy trên không
namespace COMBO_SCORES
{
	constexpr int LEVEL_1 = 100;
	constexpr int LEVEL_2 = 200;
	constexpr int LEVEL_3 = 400;
	constexpr int LEVEL_4 = 800;
	constexpr int LEVEL_5 = 1000;
	constexpr int LEVEL_6 = 2000;
	constexpr int LEVEL_7 = 4000;
	constexpr int LEVEL_8 = 8000;
	constexpr int MAX_LEVEL = 8;
}

// ScoreManager quản lý điểm, combo, xu, và mạng của người chơi
// Đây là singleton class lưu trữ tất cả scoring state
// Cách dùng:
//   1. Gọi ScoreManager::Get() để lấy singleton
//   2. AddScore() để thêm điểm
//   3. IncrementCombo() khi giết enemy trên không
//   4. AddCoin() khi nhặt xu
class ScoreManager
{
private:
	int comboCount = 0;      // Số enemy đã giết liên tiếp trên không
	bool isOnGround = true;   // Có đang đứng trên đất không (để reset combo)

	// Constructor private cho singleton pattern
	ScoreManager() {}
	~ScoreManager() {}

public:
	// Lấy instance singleton
	static ScoreManager& Get()
	{
		static ScoreManager instance;
		return instance;
	}

	// Ngăn chặn copying (singleton pattern)
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;

	// --- CORE SCORE API ---
	// Thêm điểm (cap tại 999999)
	void AddScore(int value);
	
	// Lấy điểm hiện tại
	int GetScore();
	
	// Reset điểm về 0
	void ResetScore();

	// --- COMBO SYSTEM ---
	// Tăng combo count (khi giết enemy trên không)
	void IncrementCombo();
	
	// Reset combo về 0
	void ResetCombo();
	
	// Lấy điểm combo hiện tại
	int GetComboScore();
	
	// Kiểm tra đã đạt max combo chưa
	bool IsMaxCombo();

	// --- GROUND STATE ---
	// Set trạng thái on ground (reset combo khi chạm đất)
	void SetOnGround(bool onGround);
	
	// Kiểm tra có đang on ground không
	bool IsOnGround() const;

	// --- COIN SYSTEM ---
	// Thêm xu (mặc định 1)
	void AddCoin(int amount = 1);
	
	// Lấy số xu hiện tại
	int GetCoins();

	// --- LIFE SYSTEM ---
	// Thêm mạng (mặc định 1)
	void AddLife(int amount = 1);
	
	// Lấy số mạng hiện tại
	int GetLives();

	// --- RESET ---
	// Reset tất cả scoring state (cho game mới)
	void ResetAll();

	// --- DEBUG/DISPLAY ---
	// Lấy combo count hiện tại (cho debug/display)
	int GetComboCount() const { return comboCount; }
};
