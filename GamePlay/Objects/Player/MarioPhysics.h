#pragma once
#include <Windows.h>
#include <cmath>

struct MarioPhysicsConfig
{

	float maxWalkSpeed;
	float maxRunSpeed;

	float accelWalk;
	float accelRun;
	float frictionGround;
	float accelAir;

	float skidMultiplier;

	DWORD runActivationTime;


	float jumpForceStand;
	float jumpForceWalk;
	float jumpForceRun;

	float jumpSpeedThresholdWalk;
	float jumpSpeedThresholdRun;

	float gravityRising;
	float gravityFalling;

	float terminalVelocity;

	float skidMinSpeed;


	static MarioPhysicsConfig GetDefault()
	{
		MarioPhysicsConfig config;

		config.maxWalkSpeed = 0.12f;
		config.maxRunSpeed = 0.20f;
		config.accelWalk = 0.006f;
		config.accelRun = 0.010f;
		config.frictionGround = 0.016f;
		config.accelAir = 0.004f;
		config.skidMultiplier = 0.04f;
		config.runActivationTime = 200;
		config.skidMinSpeed = 0.03f;

		config.jumpForceStand = 0.55f;
		config.jumpForceWalk = 0.62f;
		config.jumpForceRun = 0.72f;
		config.jumpSpeedThresholdWalk = 0.04f;
		config.jumpSpeedThresholdRun = 0.13f;

		config.gravityRising = 0.0025f;
		config.gravityFalling = 0.0050f;
		config.terminalVelocity = 0.50f;

		return config;
	}
};

struct MarioPhysicsInput
{
	int moveDirection;
	bool jumpPressed;
	bool jumpJustPressed;
	bool runHeld;
};

struct MarioPhysicsState
{
	float vx;
	float vy;
	bool isOnGround;
	int facing;
	bool isRunning;
	bool isSkidding;
	DWORD runHoldTime;
	float maxSpeedCap;
};

class MarioPhysics
{
private:
	MarioPhysicsConfig config;
	MarioPhysicsState state;

	bool hasReachedPeak;

public:
	MarioPhysics();
	explicit MarioPhysics(const MarioPhysicsConfig& config);

	void Update(DWORD dt, const MarioPhysicsInput& input);

	bool TryJump(bool& jumpRequested);

	const MarioPhysicsState& GetState() const { return state; }
	void SetState(const MarioPhysicsState& newState) { state = newState; }
	void SetPosition(float x, float y) {  }
	void SetOnGround(bool onGround);
	void SetFacing(int facing) { state.facing = facing; }

	const MarioPhysicsConfig& GetConfig() const { return config; }
	void SetConfig(const MarioPhysicsConfig& newConfig) { config = newConfig; }

	void Reset();

private:
	void UpdateHorizontalMovement(DWORD dt, const MarioPhysicsInput& input);

	void UpdateJumpAndGravity(DWORD dt, const MarioPhysicsInput& input);

	float GetMaxSpeed() const;
	float GetGroundAcceleration() const;
	float GetFriction(bool isAirborne) const;
	float GetJumpForce() const;
	bool ShouldSkid(int moveDirection) const;
};
