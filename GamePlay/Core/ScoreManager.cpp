#include "ScoreManager.h"
#include "debug.h"

void ScoreManager::AddScore(int value)
{
	if (value <= 0) return;

	PlayerData& data = PlayerData::Get();
	
	// Cap score at 999999
	if (data.score + value > 999999)
	{
		data.score = 999999;
	}
	else
	{
		data.score += value;
	}
}

int ScoreManager::GetScore()
{
	return PlayerData::Get().score;
}

void ScoreManager::ResetScore()
{
	PlayerData::Get().score = 0;
}

void ScoreManager::IncrementCombo()
{
	comboCount++;
	if (comboCount > COMBO_SCORES::MAX_LEVEL)
	{
		comboCount = COMBO_SCORES::MAX_LEVEL;
	}
}

void ScoreManager::ResetCombo()
{
	comboCount = 0;
}

int ScoreManager::GetComboScore()
{
	if (comboCount == 0) return 0;

	switch (comboCount)
	{
	case 1: return COMBO_SCORES::LEVEL_1;
	case 2: return COMBO_SCORES::LEVEL_2;
	case 3: return COMBO_SCORES::LEVEL_3;
	case 4: return COMBO_SCORES::LEVEL_4;
	case 5: return COMBO_SCORES::LEVEL_5;
	case 6: return COMBO_SCORES::LEVEL_6;
	case 7: return COMBO_SCORES::LEVEL_7;
	case 8: return COMBO_SCORES::LEVEL_8;
	default: return COMBO_SCORES::LEVEL_8;
	}
}

bool ScoreManager::IsMaxCombo()
{
	return comboCount >= COMBO_SCORES::MAX_LEVEL;
}

void ScoreManager::SetOnGround(bool onGround)
{
	// Reset combo when landing on ground
	if (onGround && !isOnGround)
	{
		ResetCombo();
	}
	isOnGround = onGround;
}

bool ScoreManager::IsOnGround() const
{
	return isOnGround;
}

void ScoreManager::AddCoin(int amount)
{
	if (amount <= 0) return;

	PlayerData& data = PlayerData::Get();
	data.coins += amount;

	// Award 200 points per coin
	AddScore(SCORE_VALUES::COIN);

	// Award 1UP every 100 coins
	if (data.coins >= 100)
	{
		data.coins -= 100;
		AddLife(1);
	}
}

int ScoreManager::GetCoins()
{
	return PlayerData::Get().coins;
}

void ScoreManager::AddLife(int amount)
{
	if (amount <= 0) return;

	PlayerData& data = PlayerData::Get();
	data.lives += amount;

	// Cap lives at 99 (reasonable upper limit)
	if (data.lives > 99)
	{
		data.lives = 99;
	}
}

int ScoreManager::GetLives()
{
	return PlayerData::Get().lives;
}

void ScoreManager::ResetAll()
{
	PlayerData::Get().Reset();
	comboCount = 0;
	isOnGround = true;
}
