#include "Bullet.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Enemy.h"
#include "../../Cannon.h"

void Bullet::Moving(DWORD dt)
{
	// Di chuyển theo cả 2 trục để đạn đi theo đường parabol
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Trọng lực tạo đường cong parabol
	vy += BULLET_GRAVITY * dt;

	Collision::GetInstance()->Process(this, dt, coObjects);

	// Xóa khi bay quá xa so với vị trí bắn
	if (abs(this->x - startX) > BULLET_MAX_DISTANCE) {
		this->Delete();
	}
}

void Bullet::Render()
{
	int aniId = ANIMATION::BULLET_RIGHT;
	if (this->direction < 0) {
		aniId = ANIMATION::BULLET_LEFT;
	}
	AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
}

void Bullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BULLET_BBOX_WIDTH;
	b = y + BULLET_BBOX_HEIGHT;
}

void Bullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj == owner) return;        // Bỏ qua va chạm với chủ sở hữu

	if (dynamic_cast<Enemy*>(e->obj)) { // Trúng enemy -> xử lý trước khi check blocking
		OnCollisionWithEnemy(e);        // (enemy không blocking, nên phải xử lý trước)
		return;
	}

	// Trúng Cannon -> tính 1 phát, đủ 3 phát thì Cannon biến mất; đạn tan.
	if (Cannon* cannon = dynamic_cast<Cannon*>(e->obj)) {
		cannon->OnHitByBullet();
		this->Delete();
		return;
	}

	if (!e->obj->IsBlocking()) return;  // Lọt qua các đối tượng không tĩnh

	// Đụng tường theo chiều X -> biến mất.
	if (e->nx != 0) {
		this->Delete();
		return;
	}

	// Nảy lên khi chạm vật tĩnh thay vì biến mất
	if (e->ny < 0) {                    // Chạm đất -> nảy lên
		vy = -BULLET_BOUNCE_SPEED;
	}
	else if (e->ny > 0) {               // Chạm trần -> đẩy xuống
		vy = BULLET_BOUNCE_SPEED;
	}
}

void Bullet::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	if (e->obj == owner) return;        // Bỏ qua va chạm với chủ sở hữu
	if (e->obj->IsDeleted()) return;    // Bỏ qua đối tượng đã xóa

	// Chuyển enemy sang trạng thái chết (mỗi enemy tự quyết định cách chết),
	// sau đó xóa đạn.
	if (Enemy* enemy = dynamic_cast<Enemy*>(e->obj)) {
		enemy->OnHitByBullet();
	}
	this->Delete();
}

void Bullet::OnNoCollision(DWORD dt)
{
	Moving(dt);
}

