#pragma once
#include "PlayerData.h"

namespace SCORE_VALUES
{
	constexpr int GOOMBA_STOMP = 100;
	constexpr int KOOPA_STOMP = 100;
	constexpr int COIN = 200;
	constexpr int BRICK_DESTROYED = 50;
	constexpr int SUPER_MUSHROOM = 1000;
	constexpr int FIRE_FLOWER = 1000;
	constexpr int STARMAN = 1000;
	constexpr int SHELL_KICK = 100;
	constexpr int FIREBALL_KILL = 100;
	constexpr int STAR_KILL = 100;
	constexpr int QUESTION_BLOCK_COIN = 200;
	constexpr int LEVEL_COMPLETION = 0;
}

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

class ScoreManager
{
private:
	int comboCount = 0;
	bool isOnGround = true;

	ScoreManager() {}
	~ScoreManager() {}

public:
	static ScoreManager& Get()
	{
		static ScoreManager instance;
		return instance;
	}

	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;

	void AddScore(int value);

	int GetScore();

	void ResetScore();

	void IncrementCombo();

	void ResetCombo();

	int GetComboScore();

	bool IsMaxCombo();

	void SetOnGround(bool onGround);

	bool IsOnGround() const;

	void AddCoin(int amount = 1);

	int GetCoins();

	void AddLife(int amount = 1);

	int GetLives();

	void ResetAll();

	int GetComboCount() const { return comboCount; }
};
