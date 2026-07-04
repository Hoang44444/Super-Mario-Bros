#pragma once
#include "GameObject.h"
#include <cmath>

namespace DYNAMIC_PLATFORM_PARAMS
{
	constexpr float SPEED = 0.05f;
}

namespace DYNAMIC_PLATFORM_TYPE
{
	constexpr int HORIZONTAL = 0;
	constexpr int VERTICAL = 1;
}

namespace DYNAMIC_PLATFORM_BBOX
{
	constexpr float WIDTH = 48.0f;
	constexpr float HEIGHT = 32.0f;
}
// Sàn di động (ngang hoặc dọc)
// Mario có thể đứng trên sàn này và di chuyển cùng sàn
class DynamicPlatform : public GameObject
{
private:
	float speed = DYNAMIC_PLATFORM_PARAMS::SPEED;
	float minBound, maxBound;  // Giới hạn di chuyển
	float direction = 1; //1: phải(xuống), -1: trái(lên)
	float prevX;  // Vị trí X frame trước (để tính delta cho parenting Mario)
	float prevY;  // Vị trí Y frame trước (để tính delta cho parenting Mario)

	int type; // 0: ngang, 1: dọc
public:
	DynamicPlatform(float x, float y, float z, int type, int initialDirection = 1) : GameObject(x, y, z) {
		this->type = type;
		this->direction = initialDirection;

		if (type == DYNAMIC_PLATFORM_TYPE::HORIZONTAL) {
			this->minBound = x - 50;  // Giới hạn trái/phải
			this->maxBound = x + 50;
		}
		else {
			this->minBound = y - 50;  // Giới hạn trên/dưới
			this->maxBound = y + 50;
		}
	};

	virtual ~DynamicPlatform() {};

	void Movement(DWORD dt);

	bool IsCollidable() { return true; }
	bool IsBlocking() { return true; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnNoCollision(DWORD dt) override;

	// Lấy tốc độ X (chỉ di chuyển nếu là sàn ngang)
	float GetVx() const { return type == DYNAMIC_PLATFORM_TYPE::HORIZONTAL ? speed * direction : 0.0f; }
	// Lấy tốc độ Y (chỉ di chuyển nếu là sàn dọc)
	float GetVy() const { return type == DYNAMIC_PLATFORM_TYPE::VERTICAL ? speed * direction : 0.0f; }
	int GetType() const { return type; }
	float GetPrevX() const { return prevX; }
	float GetPrevY() const { return prevY; }
};

