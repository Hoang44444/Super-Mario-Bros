#pragma once
#include <Windows.h>
#include <cmath>

// Cấu hình vật lý Mario mô phỏng Super Mario Bros (NES, 1985)
// Tất cả hằng số được điều chỉnh để phù hợp với engine hiện tại (dt-based)
// nhưng giữ đúng TỈ LỆ và MỐI QUAN HỆ như bản gốc
struct MarioPhysicsConfig
{
	// ========== DI CHUYỂN NGANG ==========
	
	// Tốc độ tối đa (pixel/frame khi dt=1)
	// NES gốc: walk = 1.5, run = 2.5 (tỉ lệ 3:5)
	float maxWalkSpeed;      // Tốc độ đi bộ tối đa
	float maxRunSpeed;       // Tốc độ chạy tối đa
	
	// Gia tốc/ma sát (pixel/ms² khi dt=1ms — engine truyền dt theo ms)
	// NES gốc: chạy tăng tốc nhanh hơn đi bộ; buông phím dừng nhanh hơn accel;
	// skid (đổi hướng) chậm hơn ma sát thường để tạo cảm giác trượt.
	float accelWalk;         // Gia tốc đi bộ trên đất
	float accelRun;          // Gia tốc chạy trên đất (lớn hơn accelWalk)
	float frictionGround;    // Ma sát khi buông phím hướng (lớn hơn accelWalk)
	float accelAir;          // Gia tốc khi ở trên không (~1/2 ground)
	
	// Hệ số giảm tốc khi skid (< 1.0): đổi hướng chậm hơn ma sát thường
	float skidMultiplier;    // Nhân với accelWalk khi đang trượt
	
	// Thời gian kích hoạt chạy (ms)
	// NES gốc: cần giữ nút chạy khoảng 10-15 frame
	DWORD runActivationTime; // Thời gian giữ nút chạy để kích hoạt tốc độ chạy
	
	// ========== NHẢY & TRỌNG LỰC ==========
	
	// Lực nhảy ban đầu (pixel/frame khi dt=1)
	// NES gốc: jump force phụ thuộc tốc độ ngang
	float jumpForceStand;    // Lực nhảy khi đứng yên
	float jumpForceWalk;     // Lực nhảy khi đi bộ
	float jumpForceRun;      // Lực nhảy khi chạy
	
	// Ngưỡng tốc độ để chọn lực nhảy (pixel/frame)
	// NES gốc: dùng tốc độ tuyệt đối để quyết định
	float jumpSpeedThresholdWalk; // Ngưỡng để dùng jumpForceWalk
	float jumpSpeedThresholdRun;  // Ngưỡng để dùng jumpForceRun
	
	// Trọng lực (pixel/frame² khi dt=1)
	// NES gốc: gravity_up = 0.05, gravity_down = 0.1
	float gravityRising;     // Trọng lực khi đang bay lên (nút nhảy giữ)
	float gravityFalling;    // Trọng lực khi đang rơi (thả nút hoặc đạt đỉnh)
	
	// Tốc độ rơi tối đa (pixel/frame khi dt=1)
	// NES gốc: terminal velocity = 4.0
	float terminalVelocity;
	
	// Ngưỡng tối thiểu để kích hoạt animation trượt (skid)
	float skidMinSpeed;

	// ========== CẤU HÌNH MẶC ĐỊNH ==========
	
	static MarioPhysicsConfig GetDefault()
	{
		MarioPhysicsConfig config;
		
		// Di chuyển ngang - tỉ lệ walk:run = 3:5 như NES
		config.maxWalkSpeed = 0.12f;      // 3/5 của run speed
		config.maxRunSpeed = 0.20f;       // Tốc độ chạy tối đa
		config.accelWalk = 0.006f;        // Gia tốc đi bộ (tăng nhẹ cho cảm giác mượt hơn)
		config.accelRun = 0.010f;         // Gia tốc chạy (nhanh hơn walk ~1.7x)
		config.frictionGround = 0.016f;   // Ma sát buông phím (nhanh hơn accelWalk)
		config.accelAir = 0.004f;         // Gia tốc trên không (~1/2 walk)
		config.skidMultiplier = 0.04f;    // Skid chậm hơn ma sát (phanh gấp khi đảo hướng, ~0.5s)
		config.runActivationTime = 200;   // ~12 frame @ 60fps — giữ hướng liên tục
		config.skidMinSpeed = 0.03f;      // Ngưỡng trượt (giảm từ 0.06 để kéo dài thời gian skid)
		
		// Nhảy - 3 mức rời rạc theo tốc độ ngang lúc bấm nhảy
		config.jumpForceStand = 0.55f;     // Lực nhảy đứng yên
		config.jumpForceWalk = 0.62f;      // Lực nhảy đi bộ (khác rõ stand)
		config.jumpForceRun = 0.72f;       // Lực nhảy chạy (khác rõ walk)
		config.jumpSpeedThresholdWalk = 0.04f; // Trên ngưỡng này -> walk jump
		config.jumpSpeedThresholdRun = 0.13f;  // Trên walk max -> run jump
		
		// Trọng lực - gravity rơi mạnh hơn gravity lên
		config.gravityRising = 0.0025f;    // Trọng lực đi lên (variable jump)
		config.gravityFalling = 0.0050f;   // Trọng lực rơi (2x rising)
		config.terminalVelocity = 0.50f;    // Tốc độ rơi tối đa
		
		return config;
	}
};

// Input từ controller (không đọc trực tiếp bàn phím)
struct MarioPhysicsInput
{
	int moveDirection;      // -1 (trái), 0 (không), 1 (phải)
	bool jumpPressed;       // Nút nhảy đang được giữ
	bool jumpJustPressed;   // Nút nhảy vừa được bấm (frame này)
	bool runHeld;           // Nút chạy đang được giữ
};

// Trạng thái vật lý Mario
struct MarioPhysicsState
{
	float vx;               // Vận tốc ngang
	float vy;               // Vận tốc dọc
	bool isOnGround;        // Có đang đứng trên đất không
	int facing;             // Hướng mặt (-1 trái, 1 phải)
	bool isRunning;         // Đang ở chế độ chạy không
	bool isSkidding;        // Đang trượt (skid)
	DWORD runHoldTime;      // Thời gian giữ nút chạy
	float maxSpeedCap;      // Tốc độ tối đa hiện tại (cap khi nhảy)
};

// Hệ thống vật lý Mario
class MarioPhysics
{
private:
	MarioPhysicsConfig config;
	MarioPhysicsState state;
	
	// Trạng thái nội bộ
	bool hasReachedPeak;    // Đã đạt đỉnh quỹ đạo chưa
	
public:
	MarioPhysics();
	explicit MarioPhysics(const MarioPhysicsConfig& config);
	
	// Cập nhật vật lý mỗi frame (di chuyển ngang + trọng lực; KHÔNG xử lý nhảy)
	void Update(DWORD dt, const MarioPhysicsInput& input);

	// Thực hiện nhảy nếu đang trên đất — gọi từ Mario sau khi biết isOnGround chính xác.
	// A2: consume jumpRequested khi nhảy thành công để chống double-trigger cùng frame.
	bool TryJump(bool& jumpRequested);
	
	// Getter/Setter cho trạng thái
	const MarioPhysicsState& GetState() const { return state; }
	void SetState(const MarioPhysicsState& newState) { state = newState; }
	void SetPosition(float x, float y) { /* Position handled by GameObject */ }
	void SetOnGround(bool onGround);
	void SetFacing(int facing) { state.facing = facing; }
	
	// Reset trạng thái
	void Reset();
	
private:
	// Xử lý di chuyển ngang
	void UpdateHorizontalMovement(DWORD dt, const MarioPhysicsInput& input);
	
	// Xử lý nhảy và trọng lực
	void UpdateJumpAndGravity(DWORD dt, const MarioPhysicsInput& input);
	
	// Helper functions
	float GetMaxSpeed() const;
	float GetGroundAcceleration() const;
	float GetFriction(bool isAirborne) const;
	float GetJumpForce() const;
	bool ShouldSkid(int moveDirection) const;
};
