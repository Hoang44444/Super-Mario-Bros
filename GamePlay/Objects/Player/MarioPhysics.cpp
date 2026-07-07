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

	if (onGround)
	{
		state.maxSpeedCap = config.maxRunSpeed;
		hasReachedPeak = false;
	}
}

void MarioPhysics::Update(DWORD dt, const MarioPhysicsInput& input)
{
	UpdateHorizontalMovement(dt, input);

	UpdateJumpAndGravity(dt, input);
}

void MarioPhysics::UpdateHorizontalMovement(DWORD dt, const MarioPhysicsInput& input)
{
	bool isAirborne = !state.isOnGround;
	float maxSpeed = GetMaxSpeed();

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

	state.isSkidding = ShouldSkid(moveDir);

	if (moveDir == 0)
	{
		if (isAirborne)
		{
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

	if (moveDir != 0)
	{
		state.facing = moveDir;
	}

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
	jumpRequested = false;

	if (fabsf(state.vx) < config.maxRunSpeed)
	{
		state.maxSpeedCap = fabsf(state.vx);
	}
	return true;
}

void MarioPhysics::UpdateJumpAndGravity(DWORD dt, const MarioPhysicsInput& input)
{
	if (!state.isOnGround)
	{
		float currentGravity;

		bool isRising = state.vy < 0;
		bool jumpReleased = !input.jumpPressed;

		if (isRising && input.jumpPressed && !hasReachedPeak)
		{
			currentGravity = config.gravityRising;
		}
		else
		{
			currentGravity = config.gravityFalling;
			hasReachedPeak = true;
		}

		state.vy += currentGravity * dt;

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
	if (moveDirection == 0 || !state.isOnGround) return false;

	bool movingOpposite = (moveDirection > 0 && state.vx < 0) || (moveDirection < 0 && state.vx > 0);
	bool hasMomentum = fabsf(state.vx) > config.skidMinSpeed;

	return movingOpposite && hasMomentum;
}
