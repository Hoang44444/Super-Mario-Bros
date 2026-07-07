#pragma once
#include "GameObject.h"

#define BULLET_SPEED 0.2f
#define BULLET_GRAVITY 0.0008f       // Trọng lực kéo đạn xuống -> tạo đường cong parabol
#define BULLET_BOUNCE_SPEED 0.25f    // Tốc độ dọc khi nảy
#define BULLET_MAX_DISTANCE 400.0f   // Xóa đạn sau khi bay xa khoảng cách này
#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8
#define SCREEN_WIDTH 800.00f


// Đạn bắn ra bởi Mario hoặc Cannon
// Bay theo đường parabol, nảy khi chạm đất, biến mất khi chạm tường hoặc bay quá xa
class Bullet : public GameObject
{
protected:
	LPGAMEOBJECT owner; // Đối tượng đã bắn đạn
	float startX;       // Vị trí X nơi đạn được bắn ra
public:
	Bullet(float x, float y, int direction, LPGAMEOBJECT owner) : GameObject(x, y, z) {
		this->z = 0.5f;
		this->direction = direction;
		this->vx = BULLET_SPEED * direction;   // Tốc độ ngang
		this->vy = 0;                          // Bắn ngang -> trọng lực kéo xuống đất
		this->owner = owner;
		this->startX = x;
	};
	~Bullet() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
	void Moving(DWORD dt);
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);

};

