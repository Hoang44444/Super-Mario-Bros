#include "Mario.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Bullet.h"
#include "StaticObject.h"
#include "Enemy.h"
#include "Item.h"
#include "InvisibleObject.h"
#include "DynamicPlatform.h"
void Mario::MovementUpdate(DWORD dt) {
	// Simple movement for testing
	this->x += this->vx * dt;
	this->y += this->vy * dt;

	this->vx += this->accelX * dt;
	this->vy += this->gravity * dt;
}

void Mario::SetLevel(int level)
{
	// (x,y) là góc trên-trái, bbox nở xuống dưới. Khi đổi level mà giữ nguyên y thì
	// phần cao thêm sẽ lòi xuống lòng platform -> Mario lọt/đứng lệch.
	// Giữ nguyên ĐÁY (chân) bằng cách dời y theo chênh lệch chiều cao bbox.
	// Chiều cao phải khớp với GetBoundingBox: SMALL = 15, còn lại (BIG/FIRE/FROG) = 27.
	auto bboxHeight = [](int lv) -> float { return lv == MARIO_LEVEL::SMALL ? 15.0f : 27.0f; };
	this->y -= (bboxHeight(level) - bboxHeight(this->level));

	this->level = level;
	this->canShoot = (level == MARIO_LEVEL::FIRE);

	PlayerData::Get().level = level;   // lưu lại để giữ qua các màn
}

void Mario::TakeDamage()
{
	// Ignore hits while invincible (star power / post-hit grace) or already dying.
	if (isInvincible || state == MARIO_STATE::DIE) return;

	if (level != MARIO_LEVEL::SMALL)
	{
		SetLevel(MARIO_LEVEL::SMALL);              // shrink one tier instead of dying
		SetInvincible(MARIO_PARAMS::HIT_GRACE_TIME); // short grace so the same enemy can't re-hit
	}
	else
	{
		SetState(MARIO_STATE::DIE);                // already small -> lose a life
	}
}

void Mario::Jump() {
	if (!isOnGround) return;
	vy = -MARIO_PARAMS::JUMP_SPEED;
	isOnGround = false;
}

void Mario::ShootBullet() {
	if (this->level != MARIO_LEVEL::FIRE) return;
	float bulletX = x + (direction > 0 ? 15.0f : -8.0f);
	float bulletY = y;
	scene->AddObject(new Bullet(bulletX, bulletY, direction, this));
}

void Mario::ResolveOverlapWithPlatforms(vector<LPGAMEOBJECT>* coObjects)
{
	if (coObjects == nullptr) return;

	for (auto obj : *coObjects) {
		auto platform = dynamic_cast<DynamicPlatform*>(obj);
		if (platform == nullptr || platform->IsDeleted()) continue;

		float pl, pt, pr, pb;
		platform->GetBoundingBox(pl, pt, pr, pb);
		float ml, mt, mr, mb;
		GetBoundingBox(ml, mt, mr, mb);

		if (mr > pl && ml < pr && mt < pt && mb > pt) {
			y -= (mb - pt) + 0.01f;
			vy = platform->GetVy();
			isOnGround = true;
		}
	}
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isInvincible)
	{
		if (invincibleTime > dt) invincibleTime -= dt;
		else { invincibleTime = 0; isInvincible = false; }
	}

	ResolveOverlapWithPlatforms(coObjects);
	Collision::GetInstance()->Process(this, dt, coObjects);
	DebugOut(L"[MARIO] Update - Position: (%.2f, %.2f), Speed: (%.2f, %.2f)\n", x, y, vx, vy);
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE::WALKING_RIGHT:
		direction = 1;
		vx = (level == MARIO_LEVEL::FROG && isOnGround) ? 0 : MARIO_PARAMS::WALK_SPEED;
		break;
	case MARIO_STATE::WALKING_LEFT:
		direction = -1;
		vx = (level == MARIO_LEVEL::FROG && isOnGround) ? 0 : -MARIO_PARAMS::WALK_SPEED;
		break;
	case MARIO_STATE::JUMP:
		if (level == MARIO_LEVEL::FROG && isOnGround)
		{
			vy = -MARIO_PARAMS::FROG_JUMP_SPEED;
			vx = MARIO_PARAMS::FROG_JUMP_SPEED_X * direction;
			isOnGround = false;
		}
		else Jump();
		break;
	case MARIO_STATE::IDLE:
		vx = 0;
		break;
	case MARIO_STATE::DIE:
		vy = -MARIO_PARAMS::JUMP_SPEED;
		break;
	case MARIO_STATE::SHOOT:
		if (canShoot) ShootBullet();
		this->state = MARIO_STATE::IDLE;
		break;
	case MARIO_STATE::RUNNING_LEFT:
		vx = -MARIO_PARAMS::RUN_SPEED;
		direction = -1;
		break;
	case MARIO_STATE::RUNNING_RIGHT:
		vx = MARIO_PARAMS::RUN_SPEED;
		direction = 1;
		break;
	}
}

void Mario::MarioSmallRender(int& aniId) {
	if (!isOnGround) {
		if (direction > 0) aniId = ANIMATION::MARIO_SMALL_JUMP_WALK_RIGHT;
		else aniId = ANIMATION::MARIO_SMALL_JUMP_WALK_LEFT;
	}
	else if (vx != 0) {
		if (direction > 0) aniId = ANIMATION::MARIO_SMALL_WALKING_RIGHT;
		else aniId = ANIMATION::MARIO_SMALL_WALKING_LEFT;
	}
	else {
		if (direction > 0) aniId = ANIMATION::MARIO_SMALL_IDLE_RIGHT;
		else aniId = ANIMATION::MARIO_SMALL_IDLE_LEFT;
	}
}

void Mario::MarioBigRender(int& aniId) {
	if (!isOnGround) {
		if (direction > 0) aniId = ANIMATION::MARIO_BIG_JUMP_WALK_RIGHT;
		else aniId = ANIMATION::MARIO_BIG_JUMP_WALK_LEFT;
	}
	else if (vx != 0) {
		if (direction > 0) aniId = ANIMATION::MARIO_BIG_WALKING_RIGHT;
		else aniId = ANIMATION::MARIO_BIG_WALKING_LEFT;
	}
	else {
		if (direction > 0) aniId = ANIMATION::MARIO_BIG_IDLE_RIGHT;
		else aniId = ANIMATION::MARIO_BIG_IDLE_LEFT;
	}
}

void Mario::MarioFireRender(int& aniId)
{
	if (!isOnGround) {
		aniId = (direction > 0) ? ANIMATION::MARIO_FIRE_JUMP_WALK_RIGHT : ANIMATION::MARIO_FIRE_JUMP_WALK_LEFT;
	}
	else if (vx != 0) {
		aniId = (direction > 0) ? ANIMATION::MARIO_FIRE_WALKING_RIGHT : ANIMATION::MARIO_FIRE_WALKING_LEFT;
	}
	else {
		aniId = (direction > 0) ? ANIMATION::MARIO_FIRE_IDLE_RIGHT : ANIMATION::MARIO_FIRE_IDLE_LEFT;
	}
}

void Mario::MarioFrogRender(int& aniId)
{
	if (!isOnGround) {
		aniId = (direction > 0) ? ANIMATION::MARIO_FROG_JUMP_RIGHT : ANIMATION::MARIO_FROG_JUMP_LEFT;
	}
	else if (vx != 0) {
		aniId = (direction > 0) ? ANIMATION::MARIO_FROG_WALKING_RIGHT : ANIMATION::MARIO_FROG_WALKING_LEFT;
	}
	else {
		aniId = (direction > 0) ? ANIMATION::MARIO_FROG_IDLE_RIGHT : ANIMATION::MARIO_FROG_IDLE_LEFT;
	}
}

void Mario::Render()
{
	int aniId = -1;

	if (state == MARIO_STATE::DIE)
		aniId = ANIMATION::MARIO_DIE;
	else if (level == MARIO_LEVEL::FIRE)
		MarioFireRender(aniId);
	else if (level == MARIO_LEVEL::FROG)
		MarioFrogRender(aniId);
	else if (level == MARIO_LEVEL::BIG)
		MarioBigRender(aniId);
	else if (level == MARIO_LEVEL::SMALL)
		MarioSmallRender(aniId);

	if (aniId != -1)
	{
		LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
		if (ani != nullptr)
			ani->Render(x, y, z);
	}
}

void Mario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (level == MARIO_LEVEL::SMALL)
	{
		r = x + 13;
		b = y + 15;
	}
	else // BIG / FIRE / FROG đều cỡ lớn
	{
		r = x + 15;
		b = y + 27;
	}
}

void Mario::OnCollisionWithStaticObject(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) {
		isOnGround = true;
		vy = 0;
	}
	else if (e->ny > 0) {
		vy = 0;
	}
	else if (e->nx != 0) {
		vx = 0;
	}
		
	auto staticObject = dynamic_cast<StaticObject*>(e->obj);
	staticObject->OnMarioCollision(this, e);
}

void Mario::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	auto enemy = dynamic_cast<Enemy*>(e->obj);
	enemy->OnMarioCollison(this, e->ny);
}

void Mario::OnCollisionWithItem(LPCOLLISIONEVENT e)
{
	auto item = dynamic_cast<Item*>(e->obj);
	item->OnMarioCollision(this);
}

void Mario::OnCollisionWithInvisibleObject(LPCOLLISIONEVENT e)
{
	auto invisibleObject = dynamic_cast<InvisibleObject*>(e->obj);
	invisibleObject->OnMarioCollision(this);
}

void Mario::OnCollisionWithDynamicPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) {
		isOnGround = true;
		vy = dynamic_cast<DynamicPlatform*>(e->obj)->GetVy();
	}
	else if (e->ny > 0) {
		vy = 0;
	}
	else if (e->nx != 0) {
		vx = 0;
	}
	DebugOut(L"[MARIO] Collided with Dynamic Platform");
}

void Mario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<DynamicPlatform*>(e->obj)) {
		OnCollisionWithDynamicPlatform(e);
	}
	else if (dynamic_cast<StaticObject*>(e->obj)) {
		OnCollisionWithStaticObject(e);
	}
	else if (dynamic_cast<Enemy*>(e->obj)) {
		OnCollisionWithEnemy(e);
	}
	else if (dynamic_cast<Item*>(e->obj)) {
		OnCollisionWithItem(e);
	}
	else if (dynamic_cast<InvisibleObject*>(e->obj)) {
		OnCollisionWithInvisibleObject(e);
	}
}

void Mario::OnNoCollision(DWORD dt) {
	MovementUpdate(dt);
}