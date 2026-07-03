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
#include "../../../Resource/SoundManager.h"
#include "../../WindCycle.h"
#include "../../Core/ScoreManager.h"
#include "../../Core/MarioPhysics.h"
#include <cstdio>

static const wchar_t* MarioAnimStateName(MarioAnimState state)
{
	switch (state)
	{
	case MarioAnimState::IDLE: return L"IDLE";
	case MarioAnimState::WALKING: return L"WALKING";
	case MarioAnimState::RUNNING: return L"RUNNING";
	case MarioAnimState::SKIDDING: return L"SKIDDING";
	case MarioAnimState::JUMPING: return L"JUMPING";
	case MarioAnimState::SITTING: return L"SITTING";
	case MarioAnimState::DYING: return L"DYING";
	default: return L"UNKNOWN";
	}
}

void Mario::MovementUpdate(DWORD dt) {
	// Position only. Velocity (gravity) is integrated once per frame in Update(),
	// so it still accelerates on frames where a collision skips this path.
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

void Mario::UpdateAnimationState(DWORD dt)
{
	if (state == MARIO_STATE::DIE)
	{
		animState = MarioAnimState::DYING;
		return;
	}

	if (animDebounceTimer > dt) animDebounceTimer -= dt;
	else animDebounceTimer = 0;

	float absVx = fabsf(vx);
	MarioAnimState previousState = animState;
	MarioAnimState nextState = animState;
	const MarioPhysicsState& physState = physics.GetState();

	if (physicsInput.moveDirection != 0 || absVx > MARIO_PARAMS::IDLE_EXIT_THRESHOLD || !isOnGround)
	{
		animStopLogCaptured = false;
	}

	if (lastAnimMoveDir != 0 && physicsInput.moveDirection == 0
		&& (animState == MarioAnimState::RUNNING || animState == MarioAnimState::WALKING))
	{
		// Debug: trace after releasing movement to verify WALK/RUN -> IDLE stability.
		animReleaseLogTimer = 2000;
	}

	if (!animStopLogCaptured && animStopLogTimer == 0 && isOnGround
		&& physicsInput.moveDirection == 0 && absVx <= MARIO_PARAMS::IDLE_ENTER_THRESHOLD)
	{
		animStopLogTimer = 2000;
		animStopLogCaptured = true;
	}

	// 1. Determine next state based on physics but with HYSTERESIS
	if (!isOnGround)
	{
		nextState = MarioAnimState::JUMPING;
	}
	else if (state == MARIO_STATE::SIT)
	{
		nextState = MarioAnimState::SITTING;
	}
	else
	{
		// Idle / Walk / Run — B1: RUN cần cả isRunning, không chỉ absVx
		if (animState == MarioAnimState::IDLE || animState == MarioAnimState::SITTING || animState == MarioAnimState::JUMPING)
		{
			if (physState.isRunning && absVx > MARIO_PARAMS::RUN_START_THRESHOLD)
				nextState = MarioAnimState::RUNNING;
			else if (absVx > MARIO_PARAMS::IDLE_EXIT_THRESHOLD)
				nextState = MarioAnimState::WALKING;
			else if (state != MARIO_STATE::SIT)
				nextState = MarioAnimState::IDLE;
		}
		else if (animState == MarioAnimState::WALKING)
		{
			if (absVx < MARIO_PARAMS::IDLE_ENTER_THRESHOLD)
				nextState = MarioAnimState::IDLE;
			else if (physState.isRunning && absVx > MARIO_PARAMS::RUN_START_THRESHOLD)
				nextState = MarioAnimState::RUNNING;
		}
		else if (animState == MarioAnimState::RUNNING)
		{
			if (absVx < MARIO_PARAMS::IDLE_ENTER_THRESHOLD)
				nextState = MarioAnimState::IDLE;
			else if (!physState.isRunning || absVx < MARIO_PARAMS::WALK_BACK_THRESHOLD)
				nextState = MarioAnimState::WALKING;
		}
		
		// 2. Special case: SKIDDING (Sửa LỖI 2: dùng nguồn chân lý từ MarioPhysics)
		if (physState.isSkidding && isOnGround)
		{
			nextState = MarioAnimState::SKIDDING;
		}
		else if (animState == MarioAnimState::SKIDDING)
		{
			// Thoát skid khi physics báo không còn skid
			nextState = (absVx > MARIO_PARAMS::WALK_START_THRESHOLD) ? MarioAnimState::WALKING : MarioAnimState::IDLE;
		}
	}

	// 3. Apply DEBOUNCE — B2: SKIDDING ưu tiên để không bị walk/idle đè khi trượt
	bool isPriorityTransition = (nextState == MarioAnimState::IDLE || nextState == MarioAnimState::JUMPING || nextState == MarioAnimState::DYING
		|| nextState == MarioAnimState::SITTING || nextState == MarioAnimState::SKIDDING);
	if (nextState != animState)
	{
		if (animDebounceTimer == 0 || isPriorityTransition)
		{
			const bool leavingIdle = (animState == MarioAnimState::IDLE && nextState != MarioAnimState::IDLE);
			animState = nextState;
			animDebounceTimer = leavingIdle ? MARIO_PARAMS::IDLE_EXIT_DEBOUNCE_TIME : MARIO_PARAMS::ANIM_DEBOUNCE_TIME;
		}
	}

	// 4. Update FACING (Sửa YÊU CẦU CHUNG: lấy trực tiếp từ physics)
	animFacing = physState.facing;

	if (animReleaseLogTimer > 0 || animStopLogTimer > 0)
	{
		DebugOut(L"[MARIO_GROUND_FRAME] dt=%lu y=%.6f vy=%.6f prev=%s next=%s current=%s vx=%.6f absVx=%.6f physRunning=%d physSkidding=%d isOnGround=%d moveDir=%d debounce=%lu releaseLogMs=%lu stopLogMs=%lu\n",
			dt, y, vy, MarioAnimStateName(previousState), MarioAnimStateName(nextState), MarioAnimStateName(animState),
			vx, absVx, physState.isRunning ? 1 : 0, physState.isSkidding ? 1 : 0,
			isOnGround ? 1 : 0, physicsInput.moveDirection, animDebounceTimer, animReleaseLogTimer, animStopLogTimer);
		FILE* animLog = nullptr;
		if (_wfopen_s(&animLog, L"mario_ground_probe.log", L"a, ccs=UTF-8") == 0 && animLog != nullptr)
		{
			fwprintf(animLog, L"[MARIO_GROUND_FRAME] dt=%lu y=%.6f vy=%.6f prev=%s next=%s current=%s vx=%.6f absVx=%.6f physRunning=%d physSkidding=%d isOnGround=%d moveDir=%d debounce=%lu releaseLogMs=%lu stopLogMs=%lu\n",
				dt, y, vy, MarioAnimStateName(previousState), MarioAnimStateName(nextState), MarioAnimStateName(animState),
				vx, absVx, physState.isRunning ? 1 : 0, physState.isSkidding ? 1 : 0,
				isOnGround ? 1 : 0, physicsInput.moveDirection, animDebounceTimer, animReleaseLogTimer, animStopLogTimer);
			fclose(animLog);
		}
		animReleaseLogTimer = (animReleaseLogTimer > dt) ? (animReleaseLogTimer - dt) : 0;
		animStopLogTimer = (animStopLogTimer > dt) ? (animStopLogTimer - dt) : 0;
	}
	lastAnimMoveDir = physicsInput.moveDirection;
}

void Mario::SetLevel(int level)
{
	// (x,y) là góc trên-trái, bbox nở xuống dưới. Khi đổi level mà giữ nguyên y thì
	// phần cao thêm sẽ lòi xuống lòng platform -> Mario lọt/đứng lệch.
	// Giữ nguyên ĐÁY (chân) bằng cách dời y theo chênh lệch chiều cao bbox.
	// Chiều cao phải khớp với GetBoundingBox (giữ nguyên chân khi đổi cấp).
	auto bboxHeight = [](int lv) -> float {
		return lv == MARIO_LEVEL::SMALL ? MARIO_PARAMS::SMALL_BBOX_HEIGHT : MARIO_PARAMS::BIG_BBOX_HEIGHT;
	};
	this->y -= (bboxHeight(level) - bboxHeight(this->level));

	this->level = level;
	this->canShoot = (level == MARIO_LEVEL::FIRE);

	PlayerData::Get().level = level;   // lưu lại để giữ qua các màn

	// Đổi cấp (ăn item lên cấp / trúng đòn tụt xuống small) -> bất tử ngắn + nhấp nháy.
	SetInvincible(MARIO_PARAMS::HIT_GRACE_TIME);
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
	SoundManager::GetInstance()->PlaySFX(SFX::JUMP);
}

void Mario::ShootBullet() {
	if (this->level != MARIO_LEVEL::FIRE) return;
	float bulletX = x + (direction > 0 ? 15.0f : -8.0f);
	float bulletY = y;
	scene->AddObject(new Bullet(bulletX, bulletY, direction, this));
	SoundManager::GetInstance()->PlaySFX(SFX::FIREBALL);
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

bool Mario::CheckGroundProbe(vector<LPGAMEOBJECT>* coObjects)
{
	if (coObjects == nullptr) return false;

	// Tạo probe AABB ngay dưới chân Mario
	float ml, mt, mr, mb;
	GetBoundingBox(ml, mt, mr, mb);

	// Probe: từ đáy Mario (mb) mở rộng xuống dưới epsilon
	float probeTop = mb;
	float probeBottom = mb + MARIO_PARAMS::GROUND_PROBE_EPSILON;
	float probeLeft = ml + 1.0f;  // thu hẹp nhẹ để tránh cạnh
	float probeRight = mr - 1.0f;

	// Check overlap với tất cả blocking objects (platforms, tiles)
	for (auto obj : *coObjects) {
		if (obj == nullptr || obj->IsDeleted()) continue;
		if (!obj->IsBlocking()) continue;  // chỉ check blocking objects

		float ol, ot, or_, ob;
		obj->GetBoundingBox(ol, ot, or_, ob);

		// AABB overlap test
		if (probeRight > ol && probeLeft < or_ && probeBottom > ot && probeTop < ob) {
			return true;  // phát hiện đất dưới chân
		}
	}

	return false;  // không có đất dưới chân
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Death animation: pop up once, then fall straight down ignoring all collisions.
	if (state == MARIO_STATE::DIE)
	{
		vy += gravity * dt;
		MovementUpdate(dt);
		return;
	}

	if (isInvincible)
	{
		if (invincibleTime > dt) invincibleTime -= dt;
		else { invincibleTime = 0; isInvincible = false; isStarPower = false; }
	}

	// A5: đẩy vx/vy Mario -> physics trước khi tích phân (collision frame trước có thể đã sửa trực tiếp)
	MarioPhysicsState ps = physics.GetState();
	ps.vx = vx;
	ps.vy = vy;
	physics.SetState(ps);
	physics.SetOnGround(isOnGround);
	physics.SetFacing(direction);

	bool jumpRequested = physicsInput.jumpJustPressed;
	const bool groundedBeforePhysics = isOnGround;
	bool jumpedThisFrame = false;

	// Di chuyển ngang + trọng lực (chưa nhảy)
	physics.Update(dt, physicsInput);

	// A2: nhảy lần 1 — đã trên đất từ frame trước
	if (jumpRequested && groundedBeforePhysics && level != MARIO_LEVEL::FROG)
	{
		if (physics.TryJump(jumpRequested))
		{
			jumpedThisFrame = true;
			SoundManager::GetInstance()->PlaySFX(SFX::JUMP);
		}
	}

	ps = physics.GetState();
	vx = ps.vx;
	vy = ps.vy;
	direction = ps.facing;
	isOnGround = ps.isOnGround;

	// Gió kéo Mario lùi về bên trái trong suốt đợt thổi. Dùng chung trạng thái
	// với hiệu ứng lá (WindCycle) nên lực đẩy và lá luôn đồng pha.
	if (this->isWindyScene && WindCycle::GetInstance()->IsActive())
	{
		this->vx += -0.003f * dt;
	}

	// Save previous ground state for jump logic (don't clear yet)
	bool wasOnGround = isOnGround;

	ResolveOverlapWithPlatforms(coObjects);
	Collision::GetInstance()->Process(this, dt, coObjects);

	// Ground probe: xác định isOnGround độc lập từ việc có collision event hay không
	isOnGround = CheckGroundProbe(coObjects);

	// Debug log: ground probe result
	DebugOut(L"[MARIO_GROUND_PROBE] dt=%lu y=%.6f vy=%.6f isOnGround=%d wasOnGround=%d\n", dt, y, vy, isOnGround ? 1 : 0, wasOnGround ? 1 : 0);
	FILE* groundProbeLog = nullptr;
	if (_wfopen_s(&groundProbeLog, L"mario_ground_probe.log", L"a, ccs=UTF-8") == 0 && groundProbeLog != nullptr)
	{
		fwprintf(groundProbeLog, L"[MARIO_GROUND_PROBE] dt=%lu y=%.6f vy=%.6f isOnGround=%d wasOnGround=%d\n", dt, y, vy, isOnGround ? 1 : 0, wasOnGround ? 1 : 0);
		fclose(groundProbeLog);
	}

	// A2: nhảy lần 2 sau collision — sửa cửa sổ chết khi vừa chạm đất cùng frame bấm nhảy.
	// Giữ phím Space khi tiếp đất (!wasOnGround -> isOnGround) cũng kích hoạt nhảy lại.
	bool landingIntent = jumpRequested;
	if (!landingIntent && physicsInput.jumpPressed && !wasOnGround && isOnGround)
		landingIntent = true;
	if (!jumpedThisFrame && landingIntent && isOnGround && level != MARIO_LEVEL::FROG)
	{
		ps = physics.GetState();
		ps.vx = vx;
		ps.vy = vy;
		physics.SetState(ps);
		physics.SetOnGround(true);
		if (physics.TryJump(landingIntent))
		{
			jumpRequested = false;
			isOnGround = false;
			ps = physics.GetState();
			vx = ps.vx;
			vy = ps.vy;
			SoundManager::GetInstance()->PlaySFX(SFX::JUMP);
		}
	}

	// A5: đẩy vx/vy sau collision ngược vào physics (tường có thể zero vx trên Mario)
	ps = physics.GetState();
	ps.vx = vx;
	ps.vy = vy;
	physics.SetState(ps);
	physics.SetOnGround(isOnGround);

	// Notify ScoreManager of ground state changes for combo system
	if (isOnGround != wasOnGround)
	{
		ScoreManager::Get().SetOnGround(isOnGround);
	}

	UpdateAnimationState(dt);

	DebugOut(L"[MARIO] Update - Position: (%.2f, %.2f), Speed: (%.2f, %.2f)\n", x, y, vx, vy);
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE::WALKING_RIGHT:
		direction = 1;
		// Physics system handles velocity - don't override
		break;
	case MARIO_STATE::WALKING_LEFT:
		direction = -1;
		// Physics system handles velocity - don't override
		break;
	case MARIO_STATE::JUMP:
		if (level == MARIO_LEVEL::FROG && isOnGround)
		{
			// A3: Frog nhảy riêng — đồng bộ physics state, isOnGround=false tránh vy bị zero
			vy = -MARIO_PARAMS::FROG_JUMP_SPEED;
			vx = MARIO_PARAMS::FROG_JUMP_SPEED_X * direction;
			isOnGround = false;
			MarioPhysicsState ps = physics.GetState();
			ps.vy = vy;
			ps.vx = vx;
			ps.isOnGround = false;
			physics.SetState(ps);
			SoundManager::GetInstance()->PlaySFX(SFX::JUMP);
		}
		break;
	case MARIO_STATE::IDLE:
		// Physics system handles deceleration
		break;
	case MARIO_STATE::DIE:
		vx = 0;                          // fall straight down, no horizontal drift
		vy = -MARIO_PARAMS::JUMP_SPEED;
		SoundManager::GetInstance()->StopBGM();
		SoundManager::GetInstance()->PlaySFX(SFX::DIE);
		break;
	case MARIO_STATE::SHOOT:
		if (canShoot) ShootBullet();
		this->state = MARIO_STATE::IDLE;
		break;
	case MARIO_STATE::RUNNING_LEFT:
		direction = -1;
		// Physics system handles velocity via runHeld input
		break;
	case MARIO_STATE::RUNNING_RIGHT:
		direction = 1;
		// Physics system handles velocity via runHeld input
		break;
	}
}

void Mario::MarioSmallRender(int& aniId) {
	switch (animState)
	{
	case MarioAnimState::JUMPING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_SMALL_JUMP_WALK_RIGHT : ANIMATION::MARIO_SMALL_JUMP_WALK_LEFT;
		break;
	case MarioAnimState::RUNNING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_SMALL_RUNNING_RIGHT : ANIMATION::MARIO_SMALL_RUNNING_LEFT;
		break;
	case MarioAnimState::WALKING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_SMALL_WALKING_RIGHT : ANIMATION::MARIO_SMALL_WALKING_LEFT;
		break;
	case MarioAnimState::SKIDDING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_SMALL_BRACE_RIGHT : ANIMATION::MARIO_SMALL_BRACE_LEFT;
		break;
	case MarioAnimState::IDLE:
	default:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_SMALL_IDLE_RIGHT : ANIMATION::MARIO_SMALL_IDLE_LEFT;
		break;
	}
}

void Mario::MarioBigRender(int& aniId) {
	switch (animState)
	{
	case MarioAnimState::JUMPING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_BIG_JUMP_WALK_RIGHT : ANIMATION::MARIO_BIG_JUMP_WALK_LEFT;
		break;
	case MarioAnimState::RUNNING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_BIG_RUNNING_RIGHT : ANIMATION::MARIO_BIG_RUNNING_LEFT;
		break;
	case MarioAnimState::WALKING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_BIG_WALKING_RIGHT : ANIMATION::MARIO_BIG_WALKING_LEFT;
		break;
	case MarioAnimState::SKIDDING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_BIG_BRACE_RIGHT : ANIMATION::MARIO_BIG_BRACE_LEFT;
		break;
	case MarioAnimState::SITTING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_BIG_SIT_RIGHT : ANIMATION::MARIO_BIG_SIT_LEFT;
		break;
	case MarioAnimState::IDLE:
	default:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_BIG_IDLE_RIGHT : ANIMATION::MARIO_BIG_IDLE_LEFT;
		break;
	}
}

void Mario::MarioFireRender(int& aniId)
{
	switch (animState)
	{
	case MarioAnimState::JUMPING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FIRE_JUMP_WALK_RIGHT : ANIMATION::MARIO_FIRE_JUMP_WALK_LEFT;
		break;
	case MarioAnimState::RUNNING:
	case MarioAnimState::WALKING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FIRE_WALKING_RIGHT : ANIMATION::MARIO_FIRE_WALKING_LEFT;
		break;
	case MarioAnimState::SKIDDING:
		// Fallback to walking if no FIRE_BRACE exists
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FIRE_WALKING_RIGHT : ANIMATION::MARIO_FIRE_WALKING_LEFT;
		break;
	case MarioAnimState::IDLE:
	default:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FIRE_IDLE_RIGHT : ANIMATION::MARIO_FIRE_IDLE_LEFT;
		break;
	}
}

void Mario::MarioFrogRender(int& aniId)
{
	switch (animState)
	{
	case MarioAnimState::JUMPING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FROG_JUMP_RIGHT : ANIMATION::MARIO_FROG_JUMP_LEFT;
		break;
	case MarioAnimState::RUNNING:
	case MarioAnimState::WALKING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FROG_WALKING_RIGHT : ANIMATION::MARIO_FROG_WALKING_LEFT;
		break;
	case MarioAnimState::IDLE:
	default:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FROG_IDLE_RIGHT : ANIMATION::MARIO_FROG_IDLE_LEFT;
		break;
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
		// Nhấp nháy khi bất tử (biến hình / ăn item / grace sau khi trúng đòn / ngôi sao):
		// bỏ vẽ ở các khung ~60ms xen kẽ để sprite chớp tắt.
		if (isInvincible && (invincibleTime / 60) % 2 == 0)
			return;

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
		r = x + MARIO_PARAMS::SMALL_BBOX_WIDTH;
		b = y + MARIO_PARAMS::SMALL_BBOX_HEIGHT;
	}
	else // BIG / FIRE / FROG đều cỡ lớn
	{
		r = x + MARIO_PARAMS::BIG_BBOX_WIDTH;
		b = y + MARIO_PARAMS::BIG_BBOX_HEIGHT;
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
		vx = 0; // A5: zero vx trên Mario; cuối Update đồng bộ ngược vào physics.state
	}
		
	auto staticObject = dynamic_cast<StaticObject*>(e->obj);
	staticObject->OnMarioCollision(this, e);
}

void Mario::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	auto enemy = dynamic_cast<Enemy*>(e->obj);
	if (enemy == nullptr) return;

	// Star power: chạm enemy nào là giết enemy đó (dùng cơ chế chết như trúng đạn).
	// Lúc bị hạ cấp chỉ có grace (isInvincible) chứ không có isStarPower nên enemy không chết.
	if (isStarPower)
	{
		enemy->OnHitByStar(this);
		SoundManager::GetInstance()->PlaySFX(SFX::STOMP);
		return;
	}

	enemy->OnMarioCollison(this, e->ny);

	// Play stomp sound if Mario is falling on top of enemy
	if (e->ny < 0)
	{
		SoundManager::GetInstance()->PlaySFX(SFX::STOMP);
	}
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
