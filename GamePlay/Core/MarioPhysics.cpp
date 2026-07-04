#include "MarioPhysics.h"
#include <cmath>
#include <cstdio>
#include <cwchar>
#include "../../Resource/debug.h"

// Constructor mặc định - dùng config mặc định
MarioPhysics::MarioPhysics()
	: config(MarioPhysicsConfig::GetDefault())
{
	Reset();
}

// Constructor với config tùy chỉnh
MarioPhysics::MarioPhysics(const MarioPhysicsConfig& config)
	: config(config)
{
	Reset();
}

// Reset trạng thái vật lý về giá trị mặc định
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

// Set trạng thái on ground
// Khi chạm đất, reset cap về tốc độ tối đa bình thường
void MarioPhysics::SetOnGround(bool onGround)
{
	state.isOnGround = onGround;
	
	if (onGround)
	{
		state.maxSpeedCap = config.maxRunSpeed;
		hasReachedPeak = false;
	}
}

// Update vật lý mỗi frame
// Xử lý di chuyển ngang và trọng lực (nhảy được xử lý riêng trong TryJump)
void MarioPhysics::Update(DWORD dt, const MarioPhysicsInput& input)
{
	// Xử lý di chuyển ngang
	UpdateHorizontalMovement(dt, input);
	
	// Xử lý trọng lực (nhảy do Mario::Update gọi TryJump() khi biết isOnGround)
	UpdateJumpAndGravity(dt, input);
}

// Xử lý di chuyển ngang
// Tính toán vận tốc ngang dựa trên input và trạng thái hiện tại
void MarioPhysics::UpdateHorizontalMovement(DWORD dt, const MarioPhysicsInput& input)
{
	bool isAirborne = !state.isOnGround;
	float maxSpeed = GetMaxSpeed();
	
	// Kích hoạt chạy: giữ phím hướng liên tục đủ lâu (tự động chuyển sang run sau runActivationTime ms)
	// Lưu ý: Bản gốc SMB (NES) dùng nút B để chạy, đây là thiết kế riêng của project
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

	// Tính isSkidding TRƯỚC KHI update vận tốc (dùng vx hiện tại của frame này)
	state.isSkidding = ShouldSkid(moveDir);

	// Buông phím: ma sát nhanh (friction > accel bình thường)
	if (moveDir == 0)
	{
		// Khi ở trên không trung và buông phím: bảo toàn đà ngang (SMB original behavior)
		// Không áp dụng friction để giữ nguyên vận tốc ngang khi nhảy
		if (isAirborne)
		{
			// Không làm gì - giữ nguyên vx
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
		// Skid: giảm tốc chậm hơn (multiplier < 1)
		float effectiveAccel = state.isSkidding ? baseAccel * config.skidMultiplier : baseAccel;

		// Nếu di chuyển cùng hướng với vận tốc hiện tại
		if ((moveDir > 0 && state.vx >= 0) || (moveDir < 0 && state.vx <= 0))
		{
			state.vx += moveDir * effectiveAccel * dt;

			// Cap tại maxSpeed
			if (fabsf(state.vx) > maxSpeed)
			{
				state.vx = moveDir * maxSpeed;
			}
		}
		else
		{
			// Đang đảo hướng - chỉ áp dụng accel (không cap)
			state.vx += moveDir * effectiveAccel * dt;
		}
	}

	// Cập nhật facing ngay lập tức theo input (giống Mario cổ điển)
	if (moveDir != 0)
	{
		state.facing = moveDir;
	}
	
	// Cap tốc độ theo maxSpeedCap (giới hạn tốc độ ngang khi nhảy)
	if (fabsf(state.vx) > state.maxSpeedCap)
	{
		state.vx = (state.vx > 0) ? state.maxSpeedCap : -state.maxSpeedCap;
	}
}

// Thử nhảy - được gọi khi Mario biết chắc chắn đang on ground
// jumpRequested: edge trigger (chỉ true trong frame đầu tiên khi nhấn phím)
bool MarioPhysics::TryJump(bool& jumpRequested)
{
	if (!jumpRequested || !state.isOnGround) return false;

	// Set vận tốc dọc theo lực nhảy
	state.vy = -GetJumpForce();
	state.isOnGround = false;
	hasReachedPeak = false;
	jumpRequested = false; // A2: consume edge ngay khi nhảy thành công

	// Cap tốc độ ngang khi nhảy (giữ nguyên tốc độ hiện tại nếu < maxRunSpeed)
	if (fabsf(state.vx) < config.maxRunSpeed)
	{
		state.maxSpeedCap = fabsf(state.vx);
	}
	return true;
}

// Xử lý trọng lực (không xử lý nhảy - nhảy được xử lý trong TryJump)
void MarioPhysics::UpdateJumpAndGravity(DWORD dt, const MarioPhysicsInput& input)
{
	// Xử lý trọng lực (A2: nhảy tách ra TryJump(), không xử lý ở đây)
	if (!state.isOnGround)
	{
		float currentGravity;
		
		// Kiểm tra xem đã đạt đỉnh quỹ đạo chưa
		bool isRising = state.vy < 0;
		bool jumpReleased = !input.jumpPressed;
		
		// Nếu đang bay lên VÀ nút nhảy vẫn được giữ -> dùng gravity nhẹ (variable jump)
		if (isRising && input.jumpPressed && !hasReachedPeak)
		{
			currentGravity = config.gravityRising;
		}
		// Nếu đã đạt đỉnh HOẶC thả nút nhảy -> dùng gravity nặng
		else
		{
			currentGravity = config.gravityFalling;
			hasReachedPeak = true;
		}
		
		// Áp dụng trọng lực
		state.vy += currentGravity * dt;
		
		// Cap tốc độ rơi tối đa
		if (state.vy > config.terminalVelocity)
		{
			state.vy = config.terminalVelocity;
		}
	}
	else
	{
		// Trên đất - không có vận tốc dọc
		state.vy = 0;
	}
}

// Lấy tốc độ tối đa dựa trên trạng thái chạy/đi bộ
float MarioPhysics::GetMaxSpeed() const
{
	if (state.isRunning)
	{
		return config.maxRunSpeed;
	}
	return config.maxWalkSpeed;
}

// Lấy gia tốc trên đất dựa trên trạng thái chạy/đi bộ
float MarioPhysics::GetGroundAcceleration() const
{
	return state.isRunning ? config.accelRun : config.accelWalk;
}

// Lấy ma sát dựa trên trạng thái trên không/trên đất
float MarioPhysics::GetFriction(bool isAirborne) const
{
	if (isAirborne)
	{
		return config.accelAir;
	}
	return config.frictionGround;
}

// Lấy lực nhảy dựa trên tốc độ ngang hiện tại
// Tốc độ càng nhanh -> nhảy càng cao
float MarioPhysics::GetJumpForce() const
{
	float speed = fabsf(state.vx);
	
	// Chọn lực nhảy dựa trên tốc độ ngang hiện tại
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

// Kiểm tra xem có nên skid không
// Skid khi di chuyển ngược với hướng vận tốc thực tế
bool MarioPhysics::ShouldSkid(int moveDirection) const
{
	// Skid khi di chuyển ngược với hướng vận tốc thực tế
	// So sánh với dấu vx, KHÔNG dùng facing (để tránh deadlock)
	if (moveDirection == 0 || !state.isOnGround) return false;

	bool movingOpposite = (moveDirection > 0 && state.vx < 0) || (moveDirection < 0 && state.vx > 0);
	bool hasMomentum = fabsf(state.vx) > config.skidMinSpeed;

	return movingOpposite && hasMomentum;
}
