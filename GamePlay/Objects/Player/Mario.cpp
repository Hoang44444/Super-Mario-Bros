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
#include "WindCycle.h"
#include "../../Core/ScoreManager.h"
#include "../../Core/MarioPhysics.h"
#include "../../Core/GameManager.h"
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
		animReleaseLogTimer = 2000;
	}

	if (!animStopLogCaptured && animStopLogTimer == 0 && isOnGround
		&& physicsInput.moveDirection == 0 && absVx <= MARIO_PARAMS::IDLE_ENTER_THRESHOLD)
	{
		animStopLogTimer = 2000;
		animStopLogCaptured = true;
	}

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

		if (physState.isSkidding && isOnGround)
		{
			nextState = MarioAnimState::SKIDDING;
		}
		else if (animState == MarioAnimState::SKIDDING)
		{
			nextState = (absVx > MARIO_PARAMS::WALK_START_THRESHOLD) ? MarioAnimState::WALKING : MarioAnimState::IDLE;
		}
	}

	static MarioAnimState lastLoggedState = MarioAnimState::IDLE;
	if (animState != lastLoggedState)
	{

		lastLoggedState = animState;
	}

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

	animFacing = physState.facing;

	if (animReleaseLogTimer > 0 || animStopLogTimer > 0)
	{
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

void Mario::SetStarPower(DWORD duration)
{
	int sceneId = GameManager::GetInstance()->GetCurrentSceneID();
	if (sceneId == SCENE::WORLD_1_2)
		previousBGM = BGM::UNDERWORLD_THEME;
	else if (sceneId == SCENE::WORLD_1_4)
		previousBGM = BGM::CASTLE_THEME;
	else
		previousBGM = BGM::OVERWORLD_THEME;

	DebugOut(L"[STAR_BGM_DEBUG] Star power activated, saved previous BGM=%d, playing STAR_THEME\n", previousBGM);

	SoundManager::GetInstance()->PlayBGM(BGM::STAR_THEME, true);

	MarioPhysicsState ps = physics.GetState();
	MarioPhysicsConfig config = physics.GetConfig();
	originalMaxRunSpeed = config.maxRunSpeed;
	config.maxRunSpeed = 0.30f;
	physics.SetConfig(config);

	isStarPower = true;
	SetInvincible(duration);
}

void Mario::SetLevel(int level)
{
	auto bboxHeight = [](int lv) -> float {
		return lv == MARIO_LEVEL::SMALL ? MARIO_PARAMS::SMALL_BBOX_HEIGHT : MARIO_PARAMS::BIG_BBOX_HEIGHT;
	};
	this->y -= (bboxHeight(level) - bboxHeight(this->level));

	this->level = level;
	this->canShoot = (level == MARIO_LEVEL::FIRE);

	PlayerData::Get().level = level;

	SetInvincible(MARIO_PARAMS::HIT_GRACE_TIME);
}

void Mario::TakeDamage()
{
	if (isInvincible || state == MARIO_STATE::DIE) return;

	if (level != MARIO_LEVEL::SMALL)
	{
		SetLevel(MARIO_LEVEL::SMALL);
		SetInvincible(MARIO_PARAMS::HIT_GRACE_TIME);
	}
	else
	{
		SetState(MARIO_STATE::DIE);
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

	float ml, mt, mr, mb;
	GetBoundingBox(ml, mt, mr, mb);

	float probeTop = mb;
	float probeBottom = mb + MARIO_PARAMS::GROUND_PROBE_EPSILON;
	float probeLeft = ml + 1.0f;
	float probeRight = mr - 1.0f;

	for (auto obj : *coObjects) {
		if (obj == nullptr || obj->IsDeleted()) continue;
		if (!obj->IsBlocking()) continue;

		float ol, ot, or_, ob;
		obj->GetBoundingBox(ol, ot, or_, ob);

		if (probeRight > ol && probeLeft < or_ && probeBottom > ot && probeTop < ob) {
			return true;
		}
	}

	return false;
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MARIO_STATE::DIE)
	{
		vy += gravity * dt;
		MovementUpdate(dt);
		return;
	}

	if (flyMode)
	{
		physics.Update(dt, physicsInput);
		y = 150.0f;
		vy = 0.0f;
		MovementUpdate(dt);
		return;
	}

	if (isInvincible)
	{
		if (invincibleTime > dt) invincibleTime -= dt;
		else
		{
			if (isStarPower)
			{
				DebugOut(L"[STAR_BGM_DEBUG] Star power ending, restoring BGM=%d\n", previousBGM);
				SoundManager::GetInstance()->StopBGM();
				if (previousBGM != -1)
				{
					SoundManager::GetInstance()->PlayBGM(previousBGM, true);
				}
				previousBGM = -1;

				MarioPhysicsConfig config = physics.GetConfig();
				config.maxRunSpeed = originalMaxRunSpeed;
				physics.SetConfig(config);
			}
			invincibleTime = 0;
			isInvincible = false;
			isStarPower = false;
		}
	}


	MarioPhysicsState ps = physics.GetState();
	ps.vx = vx;
	ps.vy = vy;
	physics.SetState(ps);
	physics.SetOnGround(isOnGround);

	bool jumpRequested = physicsInput.jumpJustPressed;
	const bool groundedBeforePhysics = isOnGround;
	bool jumpedThisFrame = false;

	physics.Update(dt, physicsInput);

	if (currentPlatform != nullptr) {
		ps = physics.GetState();
		ps.vy = 0;
		physics.SetState(ps);
	}

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

	if (this->isWindyScene && WindCycle::GetInstance()->IsActive())
	{
		constexpr float WIND_ACCEL = 0.001f;
		constexpr float WIND_MAX_FORCE = 0.08f;

		float windForce = -WIND_ACCEL * dt;
		this->vx += windForce;

		if (this->vx < -WIND_MAX_FORCE)
			this->vx = -WIND_MAX_FORCE;
	}

	bool wasOnGround = isOnGround;

	float marioYBeforeCollision = y;

	ResolveOverlapWithPlatforms(coObjects);
	Collision::GetInstance()->Process(this, dt, coObjects);

	float marioYAfterCollision = y;
	DebugOut(L"[VERTICAL_FLICKER_DEBUG] marioYBeforeCollision=%.6f marioYAfterCollision=%.6f delta=%.6f\n",
		marioYBeforeCollision, marioYAfterCollision, marioYAfterCollision - marioYBeforeCollision);

	isOnGround = CheckGroundProbe(coObjects);

	int platformCount = 0;
	for (auto obj : *coObjects) {
		if (obj == nullptr || obj->IsDeleted()) continue;
		auto platform = dynamic_cast<DynamicPlatform*>(obj);
		if (platform != nullptr) {
			float pl, pt, pr, pb;
			platform->GetBoundingBox(pl, pt, pr, pb);
			float ml, mt, mr, mb;
			GetBoundingBox(ml, mt, mr, mb);

			if (mr > pl && ml < pr && mb > pt && mt < pb) {
				platformCount++;
			}
		}
	}
	if (platformCount > 1) {
		DebugOut(L"[VERTICAL_FLICKER_DEBUG] WARNING: Double parenting detected! Mario overlapping with %d platforms\n", platformCount);
	}

	FILE* groundProbeLog = nullptr;
	if (_wfopen_s(&groundProbeLog, L"mario_ground_probe.log", L"a, ccs=UTF-8") == 0 && groundProbeLog != nullptr)
	{
		fwprintf(groundProbeLog, L"[MARIO_GROUND_PROBE] dt=%lu y=%.6f vy=%.6f isOnGround=%d wasOnGround=%d\n", dt, y, vy, isOnGround ? 1 : 0, wasOnGround ? 1 : 0);
		fclose(groundProbeLog);
	}

	constexpr float EPSILON = 1.0f;
	if (currentPlatform != nullptr) {
		float pl, pt, pr, pb;
		currentPlatform->GetBoundingBox(pl, pt, pr, pb);
		float ml, mt, mr, mb;
		GetBoundingBox(ml, mt, mr, mb);

		if (mr < pl - EPSILON || ml > pr + EPSILON || mb < pt - EPSILON || mt > pb + EPSILON) {
			currentPlatform = nullptr;
		}
	}

	if (currentPlatform != nullptr) {
		float platformX, platformY, platformZ;
		currentPlatform->GetPosition(platformX, platformY, platformZ);
		int platformType = currentPlatform->GetType();

		if (platformType == DYNAMIC_PLATFORM_TYPE::VERTICAL) {
			float deltaY = platformY - currentPlatform->GetPrevY();

			DebugOut(L"[VERTICAL_FLICKER_DEBUG] Parenting called: deltaY=%.6f\n", deltaY);

			if (deltaY < 0) {
				float ml, mt, mr, mb;
				GetBoundingBox(ml, mt, mr, mb);
				float probeTop = mt - 2.0f;
				float probeBottom = mt;

				for (auto obj : *coObjects) {
					if (obj == nullptr || obj->IsDeleted() || !obj->IsBlocking()) continue;
					if (obj == currentPlatform) continue;

					float ol, ot, or_, ob;
					obj->GetBoundingBox(ol, ot, or_, ob);

					if (mr > ol && ml < or_ && probeBottom > ot && probeTop < ob) {
						deltaY = 0;
						break;
					}
				}
			}

			float marioYBefore = y;
			y += deltaY;

			DebugOut(L"[VERTICAL_FLICKER_DEBUG] After parenting: marioYBefore=%.6f marioYAfter=%.6f delta=%.6f\n",
				marioYBefore, y, y - marioYBefore);

			DebugOut(L"[VERTICAL_PLATFORM] marioYBefore=%.6f deltaY=%.6f marioYAfter=%.6f platformTop=%.6f gap=%.6f\n",
				marioYBefore, deltaY, y, platformY, y - platformY);
		}
		else if (platformType == DYNAMIC_PLATFORM_TYPE::HORIZONTAL) {
			float deltaX = platformX - currentPlatform->GetPrevX();

			if (deltaX != 0) {
				float ml, mt, mr, mb;
				GetBoundingBox(ml, mt, mr, mb);
				float probeLeft = ml - 2.0f;
				float probeRight = mr + 2.0f;

				for (auto obj : *coObjects) {
					if (obj == nullptr || obj->IsDeleted() || !obj->IsBlocking()) continue;
					if (obj == currentPlatform) continue;

					float ol, ot, or_, ob;
					obj->GetBoundingBox(ol, ot, or_, ob);

					if (deltaX > 0 && mr > ol && ml < or_ && mb > ot && mt < ob) {
						if (or_ > mr - EPSILON && or_ < mr + 2.0f) {
							deltaX = 0;
							break;
						}
					}
					else if (deltaX < 0 && mr > ol && ml < or_ && mb > ot && mt < ob) {
						if (ol < ml + EPSILON && ol > ml - 2.0f) {
							deltaX = 0;
							break;
						}
					}
				}
			}

			float marioXBefore = x;
			x += deltaX;

			DebugOut(L"[HORIZONTAL_PLATFORM] marioXBefore=%.6f deltaX=%.6f marioXAfter=%.6f platformLeft=%.6f gap=%.6f\n",
				marioXBefore, deltaX, x, platformX, x - platformX);
		}
	}


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

	ps = physics.GetState();
	ps.vx = vx;
	ps.vy = vy;
	physics.SetState(ps);
	physics.SetOnGround(isOnGround);

	if (isOnGround != wasOnGround)
	{
		ScoreManager::Get().SetOnGround(isOnGround);
	}

	UpdateAnimationState(dt);

}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE::WALKING_RIGHT:
		direction = 1;
		break;
	case MARIO_STATE::WALKING_LEFT:
		direction = -1;
		break;
	case MARIO_STATE::JUMP:
		if (level == MARIO_LEVEL::FROG && isOnGround)
		{
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
		break;
	case MARIO_STATE::DIE:
		vx = 0;
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
		break;
	case MARIO_STATE::RUNNING_RIGHT:
		direction = 1;
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
		aniId = (animFacing > 0) ? ANIMATION::MARIO_SMALL_SKID_RIGHT : ANIMATION::MARIO_SMALL_SKID_LEFT;
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
		aniId = (animFacing > 0) ? ANIMATION::MARIO_BIG_SKID_RIGHT : ANIMATION::MARIO_BIG_SKID_LEFT;
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
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FIRE_RUNNING_RIGHT : ANIMATION::MARIO_FIRE_RUNNING_LEFT;
		break;
	case MarioAnimState::WALKING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FIRE_WALKING_RIGHT : ANIMATION::MARIO_FIRE_WALKING_LEFT;
		break;
	case MarioAnimState::SKIDDING:
		aniId = (animFacing > 0) ? ANIMATION::MARIO_FIRE_SKID_RIGHT : ANIMATION::MARIO_FIRE_SKID_LEFT;
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
	if (!isVisible)
		return;

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
		static int lastLoggedAniId = -1;
		if (aniId != lastLoggedAniId)
		{
			FILE* renderLog = nullptr;
			if (_wfopen_s(&renderLog, L"mario_skid_test.log", L"a, ccs=UTF-8") == 0 && renderLog != nullptr)
			{
				fwprintf(renderLog, L"[MARIO_ANI_ID_CHANGE] aniId: %d -> %d | level=%d animState=%s facing=%d\n",
					lastLoggedAniId, aniId, level, MarioAnimStateName(animState), animFacing);
				fclose(renderLog);
			}
			lastLoggedAniId = aniId;
		}

		if (isInvincible && !flyMode && (invincibleTime / 60) % 2 == 0)
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
	else
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
		vx = 0;
	}

	auto staticObject = dynamic_cast<StaticObject*>(e->obj);
	staticObject->OnMarioCollision(this, e);
}

void Mario::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	auto enemy = dynamic_cast<Enemy*>(e->obj);
	if (enemy == nullptr) return;

	if (isStarPower)
	{
		enemy->OnHitByStar(this);
		SoundManager::GetInstance()->PlaySFX(SFX::STOMP);
		return;
	}

	enemy->OnMarioCollison(this, e->ny);

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
	DebugOut(L"[PLATFORM_DEBUG] OnCollisionWithDynamicPlatform called: ny=%.2f nx=%.2f\n", e->ny, e->nx);

	DynamicPlatform* platform = dynamic_cast<DynamicPlatform*>(e->obj);

	if (e->ny < 0) {
		isOnGround = true;
		vy = platform->GetVy();

		if (platform != nullptr) {
			currentPlatform = platform;
		}

		if (platform != nullptr)
		{
			static float lastMarioX = x;
			static float lastMarioY = y;
			static float lastPlatformX = 0;
			static float lastPlatformY = 0;

			float marioDeltaX = x - lastMarioX;
			float marioDeltaY = y - lastMarioY;

			float platformX, platformY, platformZ;
			platform->GetPosition(platformX, platformY, platformZ);
			float platformDeltaX = platformX - lastPlatformX;
			float platformDeltaY = platformY - lastPlatformY;

			DebugOut(L"[PLATFORM_DEBUG] Mario: x=%.2f y=%.2f dx=%.6f dy=%.6f | Platform: x=%.2f y=%.2f dx=%.6f dy=%.6f | Mario.vx=%.6f Mario.vy=%.6f\n",
				x, y, marioDeltaX, marioDeltaY, platformX, platformY, platformDeltaX, platformDeltaY, vx, vy);

			lastMarioX = x;
			lastMarioY = y;
			lastPlatformX = platformX;
			lastPlatformY = platformY;
		}
	}
	else if (e->ny > 0) {
		vy = 0;
	}
	else if (e->nx != 0) {
		vx = 0;
	}
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
