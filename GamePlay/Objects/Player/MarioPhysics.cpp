#include "MarioPhysics.h"
#include <cmath>
#include <cstdio>
#include <cwchar>
#include "debug.h"

MarioPhysics::MarioPhysics()
	: config(MarioPhysicsConfig::GetDefault())
{
	Reset();
}

MarioPhysics::MarioPhysics(const MarioPhysicsConfig& config)
	: config(config)
{
	Reset();
}

void MarioPhysics::Reset()
{
	state.vx = 0.0f;
	state.vy = 0.0f;
	state.isOnGround = true;
	state.facing = 1;
	state.isRunning = false;
	state.isSkidding = false;
	state.runHoldTime = 0;
	state.maxSpeedCap = config.maxRunSpeed;
	
	hasReachedPeak = false;
}

void MarioPhysics::SetOnGround(bool onGround)
{
	state.isOnGround = onGround;
	
	// Khi cháº¡m Ä‘áº¥t, reset cap vá» tá»‘c Ä‘á»™ tá»‘i Ä‘a bÃ¬nh thÆ°á»ng
	if (onGround)
	{
		state.maxSpeedCap = config.maxRunSpeed;
		hasReachedPeak = false;
	}
}

void MarioPhysics::Update(DWORD dt, const MarioPhysicsInput& input)
{
	// Xá»­ lÃ½ di chuyá»ƒn ngang
	UpdateHorizontalMovement(dt, input);
	
	// A2: chá»‰ trá»ng lá»±c â€” nháº£y do Mario::Update gá»i TryJump() khi biáº¿t isOnGround
	UpdateJumpAndGravity(dt, input);
}

void MarioPhysics::UpdateHorizontalMovement(DWORD dt, const MarioPhysicsInput& input)
{
	bool isAirborne = !state.isOnGround;
	float maxSpeed = GetMaxSpeed();
	
	// KÃ­ch hoáº¡t cháº¡y: giá»¯ phÃ­m hÆ°á»›ng liÃªn tá»¥c Ä‘á»§ lÃ¢u (tá»± Ä‘á»™ng chuyá»ƒn sang run sau runActivationTime ms)
	// LÆ°u Ã½: Báº£n gá»‘c SMB (NES) dÃ¹ng nÃºt B Ä‘á»ƒ cháº¡y, Ä‘Ã¢y lÃ  thiáº¿t káº¿ riÃªng cá»§a project
	if (input.runHeld)
	{
		state.runHoldTime += dt;
		if (state.runHoldTime >= config.runActivationTime)
		{
			state.isRunning = true;
		}
	}
	else
	{
		state.runHoldTime = 0;
		state.isRunning = false;
	}
	
	int moveDir = input.moveDirection;

	// TÃ­nh isSkidding TRÆ¯á»šC KHI update váº­n tá»‘c (dÃ¹ng vx hiá»‡n táº¡i cá»§a frame nÃ y)
	state.isSkidding = ShouldSkid(moveDir);

	// BuÃ´ng phÃ­m: ma sÃ¡t nhanh (friction > accel bÃ¬nh thÆ°á»ng)
	if (moveDir == 0)
	{
		// Khi á»Ÿ trÃªn khÃ´ng trung vÃ  buÃ´ng phÃ­m: báº£o toÃ n Ä‘Ã  ngang (SMB original behavior)
		// KhÃ´ng Ã¡p dá»¥ng friction Ä‘á»ƒ giá»¯ nguyÃªn váº­n tá»‘c ngang khi nháº£y
		if (isAirborne)
		{
			// KhÃ´ng lÃ m gÃ¬ - giá»¯ nguyÃªn vx
		}
		else
		{
			float friction = GetFriction(isAirborne);
			if (state.vx > 0)
			{
				state.vx -= friction * dt;
				if (state.vx < 0) state.vx = 0;
			}
			else if (state.vx < 0)
			{
				state.vx += friction * dt;
				if (state.vx > 0) state.vx = 0;
			}
		}
	}
	else
	{
		float baseAccel = isAirborne ? config.accelAir : GetGroundAcceleration();
		// Skid: giáº£m tá»‘c cháº­m hÆ¡n (multiplier < 1)
		float effectiveAccel = state.isSkidding ? baseAccel * config.skidMultiplier : baseAccel;

		if ((moveDir > 0 && state.vx >= 0) || (moveDir < 0 && state.vx <= 0))
		{
			state.vx += moveDir * effectiveAccel * dt;

			if (fabsf(state.vx) > maxSpeed)
			{
				state.vx = moveDir * maxSpeed;
			}
		}
		else
		{
			state.vx += moveDir * effectiveAccel * dt;
		}
	}

	// Cáº­p nháº­t facing ngay láº­p tá»©c theo input (giá»‘ng Mario cá»• Ä‘iá»ƒn)
	if (moveDir != 0)
	{
		state.facing = moveDir;
	}
	
	// Cap tá»‘c Ä‘á»™ theo maxSpeedCap (giá»›i háº¡n tá»‘c Ä‘á»™ ngang khi nháº£y)
	if (fabsf(state.vx) > state.maxSpeedCap)
	{
		state.vx = (state.vx > 0) ? state.maxSpeedCap : -state.maxSpeedCap;
	}
}

bool MarioPhysics::TryJump(bool& jumpRequested)
{
	if (!jumpRequested || !state.isOnGround) return false;

	state.vy = -GetJumpForce();
	state.isOnGround = false;
	hasReachedPeak = false;
	jumpRequested = false; // A2: consume edge ngay khi nháº£y thÃ nh cÃ´ng

	if (fabsf(state.vx) < config.maxRunSpeed)
	{
		state.maxSpeedCap = fabsf(state.vx);
	}
	return true;
}

void MarioPhysics::UpdateJumpAndGravity(DWORD dt, const MarioPhysicsInput& input)
{
	// Xá»­ lÃ½ trá»ng lá»±c (A2: nháº£y tÃ¡ch ra TryJump(), khÃ´ng xá»­ lÃ½ á»Ÿ Ä‘Ã¢y)
	if (!state.isOnGround)
	{
		float currentGravity;
		
		// Kiá»ƒm tra xem Ä‘Ã£ Ä‘áº¡t Ä‘á»‰nh quá»¹ Ä‘áº¡o chÆ°a
		bool isRising = state.vy < 0;
		bool jumpReleased = !input.jumpPressed;
		
		// Náº¿u Ä‘ang bay lÃªn VÃ€ nÃºt nháº£y váº«n Ä‘Æ°á»£c giá»¯ -> dÃ¹ng gravity nháº¹
		if (isRising && input.jumpPressed && !hasReachedPeak)
		{
			currentGravity = config.gravityRising;
		}
		// Náº¿u Ä‘Ã£ Ä‘áº¡t Ä‘á»‰nh HOáº¶C tháº£ nÃºt nháº£y -> dÃ¹ng gravity náº·ng
		else
		{
			currentGravity = config.gravityFalling;
			hasReachedPeak = true;
		}
		
		// Ãp dá»¥ng trá»ng lá»±c
		state.vy += currentGravity * dt;
		
		// Cap tá»‘c Ä‘á»™ rÆ¡i tá»‘i Ä‘a
		if (state.vy > config.terminalVelocity)
		{
			state.vy = config.terminalVelocity;
		}
	}
	else
	{
		state.vy = 0;
	}
}

float MarioPhysics::GetMaxSpeed() const
{
	if (state.isRunning)
	{
		return config.maxRunSpeed;
	}
	return config.maxWalkSpeed;
}

float MarioPhysics::GetGroundAcceleration() const
{
	return state.isRunning ? config.accelRun : config.accelWalk;
}

float MarioPhysics::GetFriction(bool isAirborne) const
{
	if (isAirborne)
	{
		return config.accelAir;
	}
	return config.frictionGround;
}

float MarioPhysics::GetJumpForce() const
{
	float speed = fabsf(state.vx);
	
	// Chá»n lá»±c nháº£y dá»±a trÃªn tá»‘c Ä‘á»™ ngang hiá»‡n táº¡i
	if (speed >= config.jumpSpeedThresholdRun)
	{
		return config.jumpForceRun;
	}
	else if (speed >= config.jumpSpeedThresholdWalk)
	{
		return config.jumpForceWalk;
	}
	else
	{
		return config.jumpForceStand;
	}
}

bool MarioPhysics::ShouldSkid(int moveDirection) const
{
	// Skid khi di chuyá»ƒn ngÆ°á»£c vá»›i hÆ°á»›ng váº­n tá»‘c thá»±c táº¿
	// So sÃ¡nh vá»›i dáº¥u vx, KHÃ”NG dÃ¹ng facing (Ä‘á»ƒ trÃ¡nh deadlock)
	if (moveDirection == 0 || !state.isOnGround) return false;

	bool movingOpposite = (moveDirection > 0 && state.vx < 0) || (moveDirection < 0 && state.vx > 0);
	bool hasMomentum = fabsf(state.vx) > config.skidMinSpeed;

	return movingOpposite && hasMomentum;
}
