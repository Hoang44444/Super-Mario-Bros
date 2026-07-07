#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "../Core/ScoreManager.h"

// Base class cho tất cả các enemy trong game
// Xử lý va chạm với Mario, đạn, và star power
class Enemy : public GameObject
{
protected:
	ULONGLONG lastTurnTime = 0; // Lần cuối hướng bị đảo bởi turn block (ms)
public:
	Enemy(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~Enemy() {};

	bool IsCollidable() { return  true; }
	bool IsBlocking() { return false; }

	// Đảo cả hướng nhìn và tốc độ ngang, để enemy chỉ set tốc độ trong SetState() vẫn quay ngay lập tức
	void ReverseDirection() {
		direction = -direction;
		vx = -vx;
	}

	// Chỉ đảo nếu đã trôi qua cooldownMs kể từ lần đảo cuối
	// Dùng bởi EnemyTurnBlock để enemy đè lên block chỉ đảo tối đa 1 lần mỗi cooldown
	// (và tiếp tục đảo mỗi cooldown nếu vẫn ở đó)
	// Trả về true nếu thực sự đảo trong lần gọi này
	bool ReverseDirection(ULONGLONG cooldownMs) {
		ULONGLONG now = GetTickCount64();
		if (now - lastTurnTime < cooldownMs) return false;
		lastTurnTime = now;
		ReverseDirection();
		return true;
	}

	// Xử lý khi Mario va chạm với enemy (mỗi class con định nghĩa riêng)
	virtual void OnMarioCollison(Mario* mario, float ny) = 0;

	// Được gọi khi đạn trúng enemy. Mặc định: xóa enemy ngay.
	// Enemy có animation chết override để chuyển sang trạng thái chết thay vì xóa.
	virtual void OnHitByBullet() { 
		// Cộng điểm khi giết bằng đạn lửa (không combo cho fireball/star kills)
		ScoreManager::Get().AddScore(SCORE_VALUES::FIREBALL_KILL);
		Delete(); 
	}

	// Được gọi khi star power giết enemy
	virtual void OnHitByStar(Mario* mario) {
		// Cộng điểm với hệ thống combo cho star kills
		ScoreManager& scoreMgr = ScoreManager::Get();
		scoreMgr.IncrementCombo();
		
		if (scoreMgr.IsMaxCombo())
		{
			mario->AddLife(1);
		}
		else
		{
			mario->AddScore(scoreMgr.GetComboScore());
		}
		
		Delete();
	}

	// Cho phép turn block (id 44) đảo hướng enemy này không.
	// Chỉ enemy đang sống (IsCollidable() false ở trạng thái chết) và đang di chuyển (vx != 0) mới bị đảo
	// Enemy sống nhưng đứng yên (shell idle, ẩn, ...) được giữ nguyên
	virtual bool CanBeTurnedByBlock() { return IsCollidable() && vx != 0; }
};

